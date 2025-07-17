#include "glad/glad.h"
#include <GL/gl.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>
#include <memory>
#include "render/object.hh"
#include "render/scene.hh"
#include "render/shader.hh"

Scene::Scene(Camera &cam)
    : camera(cam), highlightShader("./assets/shaders/highlight.vert",
                                   "./assets/shaders/highlight.frag", false) {
  rootObject = std::make_shared<Object>("root", nullptr, Transform());
  // highlightedObject = rootObject;
}
void Scene::instantiate(std::shared_ptr<Model> model) {
  rootObject->instantiate(model);
}

void Scene::update() { camera.update(); }

void Scene::render() {
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  rootObject->cacheModelMats(Transform().getModelMat());

  Shader *curShader = nullptr;
  auto renderLambda = [&curShader](Object &o) {
    if (o.mesh != nullptr) {
      glUniformMatrix4fv(curShader->loc("model"), 1, GL_FALSE,
                         &o.getModelMat()[0][0]);
      if (o.mesh->activate()) {
        if (o.mesh->material && curShader->usesMaterial)
          o.mesh->material->activate(*curShader);

        // TODO add imgui option for style
        glDrawElements(GL_TRIANGLES, o.mesh->getAmountTris() * 3,
                       GL_UNSIGNED_INT, 0);
      } else
        std::cerr << "ERROR ACTIVATING " << o.name << std::endl;
    }
  };

  if (highlightedObject) {
    glDepthMask(GL_FALSE);
    curShader = &highlightShader;
    highlightShader.activate(camera);
    highlightedObject->apply(renderLambda);
  }

  glDepthMask(GL_TRUE);
  curShader = &camera.shader;
  camera.shader.activate(camera);
  rootObject->apply(renderLambda);

  glBindVertexArray(0);
}
