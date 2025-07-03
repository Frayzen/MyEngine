#pragma once

#include "model.hh"
#include "object.hh"
class Scene {
public:
  Scene();

  void instantiate(std::shared_ptr<Model> model);

private:
  std::shared_ptr<Object> rootObject;
};
