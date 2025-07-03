#pragma once

#include "mesh.hh"
#include "transform.hh"
#include <memory>

// A Model is a hierarchy of meshes ready to be instantiated as objects
class Model {
public:
  Model(std::shared_ptr<Model> parent, Transform &transform);

  Model loadModel(std::string &path);
  void addChild(Model &model);
  void addMesh(std::shared_ptr<Mesh> m);

  const Mesh &getMesh();
  const Transform &getTransform();

private:
  std::shared_ptr<Model> parent;
  Transform transform;
  std::vector<std::shared_ptr<Mesh>> meshes;
  std::vector<std::shared_ptr<Model>> submodels;
};
