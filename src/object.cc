#include "object.hh"
#include "glad/glad.h"
#include "mesh.hh"
#include "transform.hh"
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>

static std::shared_ptr<Object> createFromModel(std::shared_ptr<Model> model) {
  std::shared_ptr<Object> cur = std::make_shared<Object>(
      model->getName(), nullptr, model->getTransform());
  for (auto mesh : model->getMeshes()) {
    cur->children.push_back(std::make_shared<Object>(model->getName(), mesh,
                                                     model->getTransform()));
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

void Object::render(Camera &camera, glm::mat4 parentMat) {

  glm::mat4 cur = parentMat * transform.getModelMat();
  if (mesh != nullptr) {
    glUniformMatrix4fv(camera.shader.loc("model"), 1, GL_FALSE, &cur[0][0]);
    if (mesh->activate(camera))
      glDrawElements(GL_TRIANGLES, mesh->getAmountTris() * 3, GL_UNSIGNED_INT,
                     0);
  }
  // static int indent = 0;
  // indent++;
  // for (int i = 1; i < indent; i++)
  //   std::cout << "| ";
  // std::cout << name;
  // if (mesh != nullptr)
  //   std::cout << " (MESH)";
  // std::cout << std::endl;
  for (auto child : children) {
    child->render(camera, cur);
  }
  // indent--;
}
