#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
#include <ostream>
class Transform {
public:
  Transform();
  glm::vec3 position;
  glm::vec3 scale;
  glm::quat rotation;

  glm::mat4 getMat();

  glm::vec3 front();
  glm::vec3 right();
  glm::vec3 up();
};

std::ostream &operator<<(std::ostream &os, Transform const &t);
