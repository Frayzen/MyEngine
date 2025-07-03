#include "model.hh"
#include "scene.hh"
#include "window.hh"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
  std::cout << "Hello World !" << std::endl;

  Window window;
  if (window.setupWindow(800, 400, "Test"))
    return 1;
  auto m = Model::loadModel("./assets/teapot.obj");

  Scene scene;
  scene.instantiate(m);

  window.run();

  return 0;
}
