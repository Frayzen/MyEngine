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

  glm::mat4 getViewMat() const;
  glm::mat4 getModelMat() const;

  glm::vec3 front() const;
  glm::vec3 right() const;
  glm::vec3 up() const;
};

std::ostream &operator<<(std::ostream &os, Transform const &t);
