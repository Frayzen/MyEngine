#include "camera.hh"
#include "model.hh"
#include "scene.hh"
#include "window.hh"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

int main() {
  std::cout << "Hello World !" << std::endl;

  const int width = 1200;
  const int heigth = 800;

  Window window;
  if (window.setupWindow(width, heigth, "Test"))
    return 1;
  auto m =
      // Model::loadModel("./assets/big-ironman/Iron_Man_Mark_44_Hulkbuster_fbx.FBX");
      Model::loadModel("./assets/iron-man/source/Og.fbx");
  // Model::loadModel("./assets/teapot.obj");
  // Model::loadModel("./assets/Link/Link.obj");

  Camera cam(90, (1.0 * width) / heigth, 0.1, 1000);

  Scene scene(cam);
  scene.instantiate(m);

  window.run(scene);

  return 0;
}
