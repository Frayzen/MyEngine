#pragma once

#include "camera.hh"
#include "mesh.hh"
#include "model.hh"
#include "transform.hh"
#include <memory>
#include <vector>
class Object {
public:
  Object(std::shared_ptr<Mesh> mesh, const Transform transform);
  void instantiate(std::shared_ptr<Model> model);
  void render(Camera &camera, glm::mat4 viewMat);

  static std::shared_ptr<Object> createFrom(std::shared_ptr<Model> model,
                                            Object *parent);

  Transform transform;
  std::shared_ptr<Mesh> mesh;
  std::vector<std::shared_ptr<Object>> children;
};
