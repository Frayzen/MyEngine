#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include "render/camera.hh"
#include "render/model.hh"
#include "render/scene.hh"
#include "render/shader.hh"
#include "render/window.hh"

int main() {
  const int width = 1200;
  const int heigth = 800;

  Window window(width, heigth, "Test");
  auto m = //
           // Model::loadModel("./assets/iron-man/source/Og.fbx");
      Model::loadModel("./assets/teapot.obj");
  // Model::loadModel("./assets/Link/Link.obj");
  // Model::loadModel(
  // "./assets/big-ironman/Iron_Man_Mark_44_Hulkbuster_fbx.FBX");

  Camera cam(90, (1.0 * width) / heigth, 0.1, 1000);
  cam.transform.position -= glm::vec3(0, 0, 5);

  Scene scene(cam);
  scene.instantiate(m);

  window.run(scene);

  return 0;
}
