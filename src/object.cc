#include "object.hh"
#include "glad/glad.h"
#include "mesh.hh"
#include "transform.hh"
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>

static std::shared_ptr<Object> createFromModel(std::shared_ptr<Model> model,
                                               Object *parent) {
  std::shared_ptr<Object> cur =
      std::make_shared<Object>(nullptr, model->getTransform());
  parent->children.push_back(cur);
  for (auto mesh : model->getMeshes()) {
    cur->children.push_back(
        std::make_shared<Object>(mesh, model->getTransform()));
  }
  for (auto submodel : model->getSubmodels()) {
    cur->children.push_back(createFromModel(submodel, cur.get()));
  }
  return cur;
}

Object::Object(std::shared_ptr<Mesh> mesh, const Transform transform)
    : transform(transform), mesh(mesh) {}

std::shared_ptr<Object> Object::createFrom(std::shared_ptr<Model> model,
                                           Object *parent) {
  return createFromModel(model, parent);
}

void Object::instantiate(std::shared_ptr<Model> model) {
  children.push_back(Object::createFrom(model, this));
}

void Object::render(Camera &camera, glm::mat4 viewMat) {
  glm::mat4 cur = transform.getMat() * viewMat;
  if (mesh != nullptr) {
    glUniformMatrix4fv(camera.shader.loc("model"), 1, GL_FALSE, &cur[0][0]);
    mesh->activate();
    glDrawElements(GL_TRIANGLES, mesh->getAmountTris() * 3, GL_UNSIGNED_INT, 0);
  }
  for (auto child : children) {
    child->render(camera, cur);
  }
}
