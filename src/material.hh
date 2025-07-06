#pragma once

#include <assimp/material.h>
#include <glm/ext/vector_float3.hpp>
class Material {
public:
  Material(const aiMaterial *mat);

private:
  glm::vec3 Kd;
  glm::vec3 Ka;
  glm::vec3 Ks;
};
