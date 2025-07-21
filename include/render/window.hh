#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render/interface.hh"
#include "render/scene.hh"
#include "render/shader.hh"

class Window {
public:
  Window(int width, int height, const char *title);
  void run(Scene &scene);
  ~Window();

private:
  void setupFrameBuffer(int width, int height);
  GLFWwindow *window;
  std::shared_ptr<Interface> interface;

  std::shared_ptr<Shader> screenShader;

  unsigned int framebuffer;
  unsigned int textureColorbuffer;
  unsigned int rbo;
};
