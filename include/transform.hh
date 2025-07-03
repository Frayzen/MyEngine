#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
class Transform {
public:
  Transform();
  glm::vec3 position;
  glm::vec3 scale;
  glm::quat rotation;
};
