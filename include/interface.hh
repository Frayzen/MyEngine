#pragma once

#include "scene.hh"
#include <GLFW/glfw3.h>
class Interface {
public:
  Interface(GLFWwindow *window);
  void update(const Scene &scene);
  void destroy();
};
