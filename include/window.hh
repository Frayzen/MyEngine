#pragma once

#include "glad/glad.h"
#include "interface.hh"
#include "scene.hh"
#include <GLFW/glfw3.h>

class Window {
public:
  Window(int width, int height, const char *title);
  void run(Scene &scene);
  ~Window();

private:
  GLFWwindow *window;
  std::shared_ptr<Interface> interface;
};
