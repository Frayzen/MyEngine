#include "transform.hh"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <ostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

Transform::Transform()
    : position(0), scale(1), rotation(glm::identity<glm::quat>()) {}

glm::mat4 Transform::getMat() {
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
  glm::mat4 rotationMat = glm::toMat4(rotation);
  glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

  return rotationMat * scaleMat * translation;
}

std::ostream &operator<<(std::ostream &os, Transform const &t) {
  os << "SCALE: " << glm::to_string(t.scale) << std::endl;
  os << "ROT: " << glm::to_string(t.rotation) << std::endl;
  os << "POS: " << glm::to_string(t.position) << std::endl;
  return os;
}

glm::vec3 Transform::right() { return glm::vec3(1, 0, 0) * rotation; }
glm::vec3 Transform::up() { return glm::vec3(0, 1, 0) * rotation; }
glm::vec3 Transform::front() { return glm::vec3(0, 0, -1) * rotation; }
