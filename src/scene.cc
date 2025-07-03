#include "scene.hh"
#include "object.hh"
#include <memory>

Scene::Scene() { rootObject = std::make_shared<Object>(nullptr, Transform()); }
void Scene::instantiate(std::shared_ptr<Model> model) {
  rootObject->instantiate(model);
}
