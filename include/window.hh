#pragma once

#include "glad/glad.h"
#include "scene.hh"
#include <GLFW/glfw3.h>

class Window {
public:
  int setupWindow(int width, int height, const char *title);
  void run(Scene &scene);
  ~Window();

private:
  GLFWwindow *window;
};
