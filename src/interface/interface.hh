#pragma once

#include <GLFW/glfw3.h>
#include "commands/command_manager.hh"
#include "interface/console.hh"
#include "render/scene.hh"
class Interface {
private:
  CommandManager cmdManager;
  Console console;

public:
  glm::vec2 renderSize;
  Interface(Scene &scene, GLFWwindow *window, int width, int height);
  void update(Scene &scene, GLuint textureColorbuffer);
  void destroy();
};
