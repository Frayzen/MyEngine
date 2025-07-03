#include "scene.hh"
#include "glad/glad.h"
#include "object.hh"
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>

Scene::Scene(Camera &cam) : camera(cam) {
  rootObject = std::make_shared<Object>("root", nullptr, Transform());
}
void Scene::instantiate(std::shared_ptr<Model> model) {
  rootObject->instantiate(model);
}

void Scene::update() { camera.update(); }

void Scene::render() {
  camera.shader.activate();
  glm::mat4 view = camera.transform.getMat();
  glm::mat4 proj = camera.getPerspectiveMat();
  glUniformMatrix4fv(camera.shader.loc("view"), 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(camera.shader.loc("projection"), 1, GL_FALSE, &proj[0][0]);
  // std::cout << "BEGIN" << std::endl;
  rootObject->render(camera, Transform().getMat());
  // std::cout << "END" << std::endl;
  glBindVertexArray(0);
}
