#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Window {
public:
  int setupWindow(int width, int height, const char *title);
  void run();
  ~Window();

private:
  GLFWwindow *window;
};
