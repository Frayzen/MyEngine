#include "render/camera.hh"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

glm::mat4 Camera::getPerspectiveMat() const {
  return glm::perspective(fov, aspect, near, far);
}

void Camera::update() {
  // auto window = glfwGetCurrentContext();
  // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  //   transform.position -= movementSpeed * transform.front();
  // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  //   transform.position += movementSpeed * transform.front();
  // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  //   transform.position -= movementSpeed * transform.right();
  // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  //   transform.position += movementSpeed * transform.right();
  // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  //   transform.position -= movementSpeed * transform.up();
  // if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  //   transform.position += movementSpeed * transform.up();
}
