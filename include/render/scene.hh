#pragma once

#include "render/camera.hh"
#include "render/model.hh"
#include "render/object.hh"
#include "render/shader.hh"
class Scene {
public:
  Scene(Camera &cam);

  void render();
  void update();
  void instantiate(std::shared_ptr<Model> model);
  std::shared_ptr<Object> rootObject;

private:
  Camera &camera;

  Shader highlightShader;
};
