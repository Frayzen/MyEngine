#include "material.hh"
#include "utils.hh"

Material::Material(const aiMaterial *mat) {
  aiString materialName;
  aiReturn ret;
  ret = mat->Get(AI_MATKEY_NAME, materialName);
  FAIL_ON(ret != AI_SUCCESS, "Failed to load a material");

  // // Load textures
  // if ((m.ambientText_ = load(mat, aiTextureType_AMBIENT, folderRoot))) {
  //   m.ambientCol = vec3(1.0f);
  //   m.textureMask_ |= AMBIENT_TEXMASK;
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

  // Print informations
  // std::cout << "Material " << materialName.C_Str() << " loaded";
  // int textCount =
  //     (m.diffuseText_ != 0) + (m.ambientText_ != 0) + (m.specularText_ != 0);
  // std::cout << "(" << textCount << "textures)" << '\n';
}
