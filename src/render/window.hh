#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "interface/interface.hh"
#include "render/scene.hh"

class Window {
public:
  Window(int width, int height, const char *title);
  void run(Scene &scene);
  ~Window();

private:
  int width;
  int height;
  void setupFrameBuffer(int width, int height);
  GLFWwindow *window;
  std::shared_ptr<Interface> interface;

  unsigned int framebuffer;
  unsigned int textureColorbuffer;
  unsigned int rbo;
};
