#pragma once

#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include <vector>
#include "render/bounds.hh"
#include "render/mesh.hh"
#include "render/model.hh"
#include "render/transform.hh"

// An object is an instantiated Model
class Object {
public:
  Object(const std::string name, std::shared_ptr<Mesh> mesh,
         const Transform transform);

  std::shared_ptr<Object> instantiate(std::shared_ptr<Model> model);
  void apply(const std::function<int(Object &obj)> fn);
  void cacheModelMats(const glm::mat4 &parentMat);

  bool rendered;
  std::string name;
  Transform transform;
  std::shared_ptr<Mesh> mesh;

  void addChild(std::shared_ptr<Object> obj);
  const std::vector<std::shared_ptr<Object>> &getChildren() const;

  const glm::mat4 &getModelMat() const;
  const Bounds &getBounds() const;

private:
  std::vector<std::shared_ptr<Object>> children;
  glm::mat4 modelMat;
  Bounds bounds;
};
