#include "glad/glad.h"
#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include "render/mesh.hh"
#include "render/object.hh"
#include "render/shader.hh"
#include "render/transform.hh"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

static std::shared_ptr<Object> createFromModel(std::shared_ptr<Model> model) {
  std::shared_ptr<Object> cur = std::make_shared<Object>(
      model->getName(), nullptr, model->getTransform());
  for (auto mesh : model->getMeshes()) {
    cur->children.push_back(
        std::make_shared<Object>(mesh->getName(), mesh, model->getTransform()));
  }
  for (auto submodel : model->getSubmodels()) {
    cur->children.push_back(createFromModel(submodel));
  }
  return cur;
}

Object::Object(const std::string name, std::shared_ptr<Mesh> mesh,
               const Transform transform)
    : name(name), transform(transform), mesh(mesh) {}

std::shared_ptr<Object> Object::instantiate(std::shared_ptr<Model> model) {
  auto created = createFromModel(model);
  children.push_back(created);
  return created;
}

void Object::cacheModelMats(const glm::mat4 &parentMat) {
  modelMat = parentMat * transform.getModelMat();
  for (auto child : children) {
    child->cacheModelMats(modelMat);
  }
}

void Object::apply(const std::function<void(Object &obj)> fn) {
  fn(*this);
  for (auto child : children) {
    child->apply(fn);
  }
}

const glm::mat4 &Object::getModelMat() const { return modelMat; }
