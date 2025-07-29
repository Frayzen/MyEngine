#pragma once

#include "render/camera.hh"
#include "render/model.hh"
#include "render/object.hh"
#include "render/shader.hh"
class Scene {
public:
  Scene(const Scene &s) = delete;
  Scene(Camera &cam);

  void render();
  void update();
  void instantiate(std::shared_ptr<Model> model);
  std::shared_ptr<Object> rootObject;

  void updateHighlighted(Object *newHighlight,
                         glm::vec3 lookDir = glm::vec3(0, 0, -1));
  Object *getHighlighted() const;

  Camera &camera;

private:
  Shader highlightShader;
  Object *highlightedObject;
};
