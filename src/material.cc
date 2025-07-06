#include "material.hh"
#include "utils.hh"
#include <assimp/material.h>
#include <filesystem>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stbi_load.hh"

// static GLuint load(const aiMaterial *mat, aiTextureType type,
//                    std::filesystem::path folderRoot) {
//   // Diffuse maps
//   int numTextures = mat->GetTextureCount(type);
//   aiString fileName;
//   std::cout << "Texcount " << numTextures << std::endl;
//   ;
//   if (numTextures > 0) {
//     if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &fileName, NULL, NULL,
//     NULL,
//                         NULL, NULL) == AI_SUCCESS) {
//       int width, height, nrChannels;
//       auto path = folderRoot.append(fileName.C_Str());

//       auto *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
//       FAIL_ON(!data, "Error while trying to read " << path);
//       std::cout << "Successfully read " << path << std::endl;
//       GLuint buffer;
//       glGenTextures(1, &buffer);
//       glBindTexture(GL_TEXTURE_2D, buffer);

//       // set the texture wrapping/filtering options
//       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//                       GL_LINEAR_MIPMAP_LINEAR);
//       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//       int from;
//       switch (nrChannels) {
//       case 1:
//         from = GL_RED;
//         break;
//       case 3:
//         from = GL_RGB;
//         break;
//       case 4:
//         from = GL_RGBA;
//         break;
//       default:
//         FAIL_ON(true, "Unsupoorted number of channel : " << nrChannels);
//         return 0;
//       }

//       glTexImage2D(GL_TEXTURE_2D, 0, from, width, height, 0, GL_RGB,
//                    GL_UNSIGNED_BYTE, data);
//       glGenerateMipmap(GL_TEXTURE_2D);
//       glBindTexture(GL_TEXTURE_2D, 0);
//       stbi_image_free(data);
//       return buffer;
//     }
//   }
//   return 0;
// }

Material::Material(std::filesystem::path path, const aiMaterial *mat) {
  (void)path;
  aiString materialName;
  aiReturn ret;
  ret = mat->Get(AI_MATKEY_NAME, materialName);
  FAIL_ON(ret != AI_SUCCESS, "Failed to load a material");

  // // Load textures
  // if ((mapKa = load(mat, aiTextureType_AMBIENT, path))) {
  //   Ka = glm::vec3(1.0f);
  // }
  // if ((m.diffuseText_ = load(mat, aiTextureType_DIFFUSE, folderRoot))) {
  //   m.diffuseCol = vec3(1.0f);
  //   m.textureMask_ |= DIFFUSE_TEXMASK;
  // }
  // if ((m.specularText_ = load(mat, aiTextureType_SPECULAR, folderRoot))) {
  //   m.specularCol = vec3(1.0f);
  //   m.textureMask_ |= SPECULAR_TEXMASK;
  // }
  // if ((m.normalMap_ = load(mat, aiTextureType_NORMALS, folderRoot)))
  //   m.textureMask_ |= NORMAL_MAPMASK;

  // // Load colors and scalars
  // mat->Get(AI_MATKEY_TWOSIDED, m.twosided);
  // mat->Get(AI_MATKEY_SHININESS, m.shininess);
  // mat->Get(AI_MATKEY_SHININESS_STRENGTH, m.shininessStrength);
  // mat->Get(AI_MATKEY_OPACITY, m.opacity);

  aiColor3D c;
  if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, c))
    Ka = glm::vec3(c.r, c.g, c.b);
  if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, c))
    Kd = glm::vec3(c.r, c.g, c.b);
  if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, c))
    Ks = glm::vec3(c.r, c.g, c.b);
  // if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_EMISSIVE, c))
  //   m.emissiveCol = glm::vec3(c.r, c.g, c.b);

  int boolean;
  if (AI_SUCCESS == mat->Get(AI_MATKEY_TWOSIDED, boolean))
    disableCulling = boolean;

  // Print informations
  std::cout << "Material " << materialName.C_Str() << " loaded" << std::endl;
  // int textCount =
  //     (m.diffuseText_ != 0) + (m.ambientText_ != 0) + (m.specularText_ != 0);
  // std::cout << "(" << textCount << "textures)" << '\n';
}

void Material::activate() {
  if (disableCulling)
    glDisable(GL_CULL_FACE);
  else
    glEnable(GL_CULL_FACE);
}
