#pragma once

#include <GLFW/glfw3.h>
#include "render/scene.hh"
class Interface {
public:
  Interface(GLFWwindow *window);
  void update(Scene &scene);
  void destroy();
};
