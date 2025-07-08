#include "material.hh"
#include "glad/glad.h"
#include "utils.hh"
#include <assimp/material.h>
#include <filesystem>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stbi_load.hh"

static GLuint WhitePixelTexture() {
  static GLuint textureID;
  if (textureID)
    return textureID;
  unsigned char pixel[] = {255, 255, 255, 255}; // RGBA white

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               pixel);

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);
  return textureID;
}

static void applyMappingMode(const aiTextureMapMode &mode, GLuint text_wrap) {
  switch (mode) {
  case aiTextureMapMode_Wrap:
    glTexParameteri(GL_TEXTURE_2D, text_wrap, GL_REPEAT);
    break;
  case aiTextureMapMode_Clamp:
    glTexParameteri(GL_TEXTURE_2D, text_wrap, GL_CLAMP_TO_EDGE);
    break;
  case aiTextureMapMode_Decal:
    // Note: OpenGL doesn't have a direct "Decal" mode, but GL_CLAMP_TO_BORDER
    // with a transparent border can be used.
    glTexParameteri(GL_TEXTURE_2D, text_wrap, GL_CLAMP_TO_BORDER);
    break;
  case aiTextureMapMode_Mirror:
    glTexParameteri(GL_TEXTURE_2D, text_wrap, GL_MIRRORED_REPEAT);
    break;
  case _aiTextureMapMode_Force32Bit:
    // This is just a sentinel value, not a real mode. Use default (REPEAT).
    glTexParameteri(GL_TEXTURE_2D, text_wrap, GL_REPEAT);
    break;
  }
}

static GLuint load(const aiMaterial *mat, aiTextureType type,
                   std::filesystem::path folderRoot) {
  // Diffuse maps
  int numTextures = mat->GetTextureCount(type);
  aiString fileName;
  std::cout << "Texcount " << numTextures << std::endl;
  if (numTextures > 0 && mat->GetTexture(type, 0, &fileName, NULL, NULL, NULL,
                                         NULL, NULL) == AI_SUCCESS) {
    int width, height, nrChannels;
    auto path = folderRoot.append(fileName.C_Str());
    stbi_set_flip_vertically_on_load(true);
    auto *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    FAIL_ON(!data, "Error while trying to read " << path);
    std::cout << "Successfully read " << path << std::endl;
    GLuint buffer;
    glGenTextures(1, &buffer);
    glBindTexture(GL_TEXTURE_2D, buffer);

    aiTextureMapMode mode;
    mat->Get(AI_MATKEY_MAPPINGMODE_U(type, 0), mode);
    applyMappingMode(mode, GL_TEXTURE_WRAP_S);
    mat->Get(AI_MATKEY_MAPPINGMODE_V(type, 0), mode);
    applyMappingMode(mode, GL_TEXTURE_WRAP_T);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int from;
    switch (nrChannels) {
    case 1:
      from = GL_RED;
      break;
    case 3:
      from = GL_RGB;
      break;
    case 4:
      from = GL_RGBA;
      break;
    default:
      FAIL_ON(true, "Unsupoorted number of channel : " << nrChannels);
      return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, from, width, height, 0, from,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return buffer;
  }
  return WhitePixelTexture();
}

Material::Material(std::filesystem::path path, const aiMaterial *mat) {
  (void)path;
  aiString materialName;
  aiReturn ret;
  ret = mat->Get(AI_MATKEY_NAME, materialName);
  FAIL_ON(ret != AI_SUCCESS, "Failed to load a material");

  // Load textures
  mapKa = load(mat, aiTextureType_AMBIENT, path);
  mapKd = load(mat, aiTextureType_DIFFUSE, path);
  mapKs = load(mat, aiTextureType_SPECULAR, path);

  // // Load colors and scalars
  // mat->Get(AI_MATKEY_OPACITY, m.opacity);

  aiColor3D c;
  if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, c))
    Ks = glm::vec3(c.r, c.g, c.b);
  if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, c))
    Kd = glm::vec3(c.r, c.g, c.b);
  if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, c)) {
    Ka = glm::vec3(c.r, c.g, c.b);
    if (Ka == glm::vec3(1, 1, 1)) {
      // Ambient was likely unset—ignore or override.
      Ka = Kd;
      if (mapKa == WhitePixelTexture())
        mapKa = mapKd;
    }
  }

  int boolean;
  int val;
  if (AI_SUCCESS == mat->Get(AI_MATKEY_TWOSIDED, boolean))
    disableCulling = boolean;
  if (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS, val))
    shininess = val;
  if (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS_STRENGTH, val))
    shininess_strength = val;
  if (AI_SUCCESS == mat->Get(AI_MATKEY_BLEND_FUNC, val))
    additiveBlend = val == aiBlendMode::aiBlendMode_Additive;

  // Print informations
  std::cout << "Material " << materialName.C_Str() << " loaded" << std::endl;
  std::cout << "  Ka = " << glm::to_string(Ka) << std::endl;
  std::cout << "  Kd = " << glm::to_string(Kd) << std::endl;
  std::cout << "  Ks = " << glm::to_string(Ks) << std::endl;
  std::cout << "  Ks = " << glm::to_string(Ks) << std::endl;
  std::cout << "  shininess = " << shininess << std::endl;
  std::cout << "  shininess_strength = " << shininess_strength << std::endl;
  // int textCount =
  //     (m.diffuseText_ != 0) + (m.ambientText_ != 0) + (m.specularText_ != 0);
  // std::cout << "(" << textCount << "textures)" << '\n';
}

void Material::activate(const Camera &cam) {
  if (disableCulling)
    glDisable(GL_CULL_FACE);
  else
    glEnable(GL_CULL_FACE);

  if (additiveBlend)
    glBlendFunc(GL_ONE, GL_ONE);
  else
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mapKa);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, mapKd);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, mapKs);

  glUniform3f(cam.shader.loc("Ka"), Ka.r, Ka.g, Ka.b);
  glUniform3f(cam.shader.loc("Kd"), Kd.r, Kd.g, Kd.b);
  glUniform3f(cam.shader.loc("Ks"), Ks.r, Ks.g, Ks.b);

  // 0 = texture unit index (GL_TEXTURE0)
  glUniform1i(cam.shader.loc("mapKa"), 0);
  glUniform1i(cam.shader.loc("mapKd"), 1);
  glUniform1i(cam.shader.loc("mapKs"), 2);

  glUniform1f(cam.shader.loc("shininess"), shininess);
  glUniform1f(cam.shader.loc("shininess_strength"), shininess_strength);
}
