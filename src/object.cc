#include "object.hh"
#include "mesh.hh"
#include "transform.hh"

static std::shared_ptr<Object> createFromModel(std::shared_ptr<Model> model,
                                               Object *parent) {
  std::shared_ptr<Object> cur =
      std::make_shared<Object>(nullptr, model->getTransform());
  parent->children.push_back(cur);
  for (auto mesh : model->getMeshes())
    cur->children.push_back(
        std::make_shared<Object>(mesh, model->getTransform()));
  return cur;
}

Object::Object(std::shared_ptr<Mesh> mesh, const Transform transform)
    : transform(transform), mesh(mesh) {}

std::shared_ptr<Object> Object::createFrom(std::shared_ptr<Model> model,
                                           Object *parent) {
  return createFromModel(model, parent);
}

void Object::instantiate(std::shared_ptr<Model> model) {
  Object::createFrom(model, this);
}
