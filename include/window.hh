#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

class MEWindow {
public:
  int setupWindow(int width, int height, const char *title);
  void run();
  ~MEWindow();

private:
  GLFWwindow *window;
};
