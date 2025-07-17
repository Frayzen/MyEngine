#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render/interface.hh"
#include "render/scene.hh"

class Window {
public:
  Window(int width, int height, const char *title);
  void run(Scene &scene);
  ~Window();

private:
  GLFWwindow *window;
  std::shared_ptr<Interface> interface;
};
