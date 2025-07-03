#include "transform.hh"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

Transform::Transform()
    : position(0), scale(1), rotation(glm::identity<glm::quat>()) {}

glm::mat4 Transform::getMat() {
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
  glm::mat4 rotationMat = glm::toMat4(rotation);
  glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

  return translation * rotationMat * scaleMat;
}
