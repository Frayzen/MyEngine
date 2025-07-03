#include "camera.hh"
#include <glm/ext/matrix_clip_space.hpp>

glm::mat4 Camera::getPerspectiveMat() {
  return glm::perspective(fov, aspect, near, far);
}
