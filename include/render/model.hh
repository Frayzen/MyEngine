#pragma once

#include <memory>
#include "render/mesh.hh"
#include "render/transform.hh"

// A Model is a hierarchy of meshes ready to be instantiated as objects
class Model {
public:
  Model(const std::string name, std::shared_ptr<Model> parent,
        Transform &transform);

  static std::shared_ptr<Model> loadModel(const std::string path);
  void addChild(Model &model);
  void addMesh(std::shared_ptr<Mesh> m);
  void addSubmodel(std::shared_ptr<Model> submodel);

  const std::vector<std::shared_ptr<Mesh>> getMeshes();
  const std::vector<std::shared_ptr<Model>> getSubmodels();
  Transform &getTransform();
  const std::string &getName();

private:
  std::string name;
  std::shared_ptr<Model> parent;
  Transform transform;
  std::vector<std::shared_ptr<Mesh>> meshes;
  std::vector<std::shared_ptr<Model>> submodels;
};
