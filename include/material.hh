#pragma once

#include "glad/glad.h"

#include <assimp/material.h>
#include <filesystem>
#include <glm/ext/vector_float3.hpp>
class Material {
public:
  Material(std::filesystem::path path, const aiMaterial *mat);

  void activate();

private:
  glm::vec3 Kd, Ka, Ks;
  unsigned int mapKa, mapKd, mapKs;

  bool disableCulling = false;
};
