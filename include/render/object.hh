#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include <vector>
#include "render/camera.hh"
#include "render/mesh.hh"
#include "render/model.hh"
#include "render/transform.hh"
class Object {
public:
  Object(const std::string name, std::shared_ptr<Mesh> mesh,
         const Transform transform);

  std::shared_ptr<Object> instantiate(std::shared_ptr<Model> model);
  void render(Camera &camera);
  void cacheModelMats(const glm::mat4 &parentMat);

  std::string name;
  Transform transform;
  std::shared_ptr<Mesh> mesh;
  std::vector<std::shared_ptr<Object>> children;

private:
  glm::mat4 modelMat;
};
