#include "render/camera.hh"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

glm::mat4 Camera::getPerspectiveMat() const {
  return glm::perspective(fov, aspect, near, far);
}
