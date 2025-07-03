#pragma once

#include "camera.hh"
#include "model.hh"
#include "object.hh"
class Scene {
public:
  Scene(Camera &cam);

  void render();
  void instantiate(std::shared_ptr<Model> model);

private:
  Camera &camera;
  std::shared_ptr<Object> rootObject;
};
