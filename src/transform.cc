#include "transform.hh"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

Transform::Transform()
    : position(0), scale(1), rotation(glm::identity<glm::quat>()) {}
