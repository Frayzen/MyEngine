#include "render/model.hh"

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/quaternion.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <filesystem>
#include <memory>
#include <vector>
#include "render/material.hh"
#include "render/mesh.hh"
#include "render/transform.hh"
#include "render/utils.hh"

struct ModelBuilder {
  const aiScene *scene;
  std::vector<std::shared_ptr<Mesh>> meshes;
  std::vector<std::shared_ptr<Material>> materials;
};

static Transform getTransform(const aiNode *node) {
  aiVector3D pos;
  aiQuaternion rot;
  aiVector3D scale;
  node->mTransformation.Decompose(scale, rot, pos);
  Transform transform;
  transform.scale = {scale.x, scale.y, scale.z};
  transform.position = {pos.x, pos.y, pos.z};
  transform.rotation = {rot.w, rot.x, rot.y, rot.z};
  return transform;
}

static std::shared_ptr<Model> createModel(ModelBuilder &builder,
                                          std::shared_ptr<Model> parent,
                                          const aiNode *node) {
  Transform transform = getTransform(node);
  std::shared_ptr<Model> cur =
      std::make_shared<Model>(node->mName.C_Str(), parent, transform);

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    cur->addMesh(builder.meshes[node->mMeshes[i]]);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    cur->addSubmodel(createModel(builder, cur, node->mChildren[i]));
  }
  return cur;
}

std::shared_ptr<Model> Model::loadModel(const std::string path) {
  std::filesystem::path fullPath(path);

  static Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path.c_str(),
      aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
          aiProcess_MakeLeftHanded | aiProcess_GenUVCoords |
          aiProcess_RemoveRedundantMaterials |
          aiProcess_GenSmoothNormals /* or aiProcess_GenNormals */);
  FAIL_ON(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
              !scene->mRootNode,
          "The mesh " << path.c_str() << "could not be loaded.\nReason:"
                      << importer.GetErrorString());
  std::cout << "DONE LOADING " << path << '\n';

  struct ModelBuilder builder = {.scene = scene,
                                 .meshes = std::vector<std::shared_ptr<Mesh>>(),
                                 .materials =
                                     std::vector<std::shared_ptr<Material>>()};
  auto parentPath = fullPath.parent_path();
  for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    builder.materials.push_back(
        std::make_shared<Material>(parentPath, scene->mMaterials[i]));
  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    const aiMesh *aiMesh = scene->mMeshes[i];
    auto loadedMesh = std::make_shared<Mesh>(aiMesh);
    builder.meshes.push_back(loadedMesh);
    if (aiMesh->mMaterialIndex < builder.materials.size())
      loadedMesh->setMaterial(builder.materials[aiMesh->mMaterialIndex]);
    else
      std::cout << "No material for " << aiMesh->mName.C_Str() << std::endl;
  }
  return createModel(builder, nullptr, scene->mRootNode);
}

void Model::addChild(Model &model) {
  submodels.push_back(std::shared_ptr<Model>(&model));
}

void Model::addMesh(std::shared_ptr<Mesh> m) {
  meshes.push_back(m);
  bounds.expand(m->getBounds());
}
void Model::addSubmodel(std::shared_ptr<Model> model) {
  submodels.push_back(model);
  bounds.expand(model->getBounds());
}

Model::Model(std::string name, std::shared_ptr<Model> parent,
             Transform &transform)
    : bounds(), name(name), parent(parent), transform(transform) {};

const std::vector<std::shared_ptr<Mesh>> Model::getMeshes() { return meshes; }
const std::vector<std::shared_ptr<Model>> Model::getSubmodels() {
  return submodels;
}
Transform &Model::getTransform() { return transform; }

const std::string &Model::getName() { return name; }
const Bounds &Model::getBounds() const { return bounds; }
