#pragma once

#include <GLFW/glfw3.h>
#include "commands/command_manager.hh"
#include "render/scene.hh"
class Interface {
private:
  CommandManager manager;

public:
  Interface(Scene &scene, GLFWwindow *window);
  void update(Scene &scene, GLuint textureColorbuffer);
  void destroy();
};
