#pragma once

#include <assimp/material.h>
#include <filesystem>
#include <glm/ext/vector_float3.hpp>
#include "render/camera.hh"
class Material {
public:
  Material() = default;
  Material(std::filesystem::path path, const aiMaterial *mat);

  void activate(const Camera &cam);

private:
  glm::vec3 Kd, Ka, Ks = glm::vec3(0);
  unsigned int mapKa, mapKd, mapKs;
  float shininess, shininess_strength;
  float reflectivity;

  bool disableCulling, additiveBlend = false;
};
