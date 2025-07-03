#include "scene.hh"
#include "glad/glad.h"
#include "object.hh"
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>

Scene::Scene(Camera &cam) : camera(cam) {
  rootObject = std::make_shared<Object>(nullptr, Transform());
}
void Scene::instantiate(std::shared_ptr<Model> model) {
  rootObject->instantiate(model);
}

void Scene::render() {
  camera.shader.activate();
  glm::mat4 view = camera.transform.getMat();
  glm::mat4 proj = camera.getPerspectiveMat();
  glUniformMatrix4fv(camera.shader.loc("view"), 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(camera.shader.loc("projection"), 1, GL_FALSE, &proj[0][0]);
  rootObject->render(camera, Transform().getMat());
  glBindVertexArray(0);
}
