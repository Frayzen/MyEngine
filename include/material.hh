#pragma once

#include "glad/glad.h"

#include "camera.hh"
#include <assimp/material.h>
#include <filesystem>
#include <glm/ext/vector_float3.hpp>
class Material {
public:
  Material(std::filesystem::path path, const aiMaterial *mat);

  void activate(const Camera &cam);

private:
  glm::vec3 Kd, Ka, Ks;
  unsigned int mapKa, mapKd, mapKs;
  float shininess, shininess_strength = 1.0f;

  bool disableCulling = false;
};
