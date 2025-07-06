#pragma once

#include "camera.hh"
#include "mesh.hh"
#include "model.hh"
#include "transform.hh"
#include <memory>
#include <vector>
class Object {
public:
  Object(const std::string name, std::shared_ptr<Mesh> mesh,
         const Transform transform);

  std::shared_ptr<Object> instantiate(std::shared_ptr<Model> model);
  void render(Camera &camera, glm::mat4 parentMat);

  std::string name;
  Transform transform;
  std::shared_ptr<Mesh> mesh;
  std::vector<std::shared_ptr<Object>> children;
};
