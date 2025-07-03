#include "camera.hh"
#include "model.hh"
#include "scene.hh"
#include "window.hh"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
  std::cout << "Hello World !" << std::endl;

  const int width = 1800;
  const int heigth = 1200;

  Window window;
  if (window.setupWindow(width, heigth, "Test"))
    return 1;
  auto m = Model::loadModel("./assets/teapot.obj");

  Camera cam(90, (1.0 * width) / heigth, 0.1, 1000);
  cam.transform.position = glm::vec3(0, 0, -8);
  Scene scene(cam);
  scene.instantiate(m);

  window.run(scene);

  return 0;
}
