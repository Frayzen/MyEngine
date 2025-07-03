#include "model.hh"

#include "mesh.hh"
#include "transform.hh"
#include "utils.hh"
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/quaternion.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <filesystem>
#include <memory>
#include <vector>

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

static std::shared_ptr<Model> createModel(const aiScene *scene,
                                          std::shared_ptr<Model> parent,
                                          const aiNode *node) {
  Transform transform = getTransform(node);
  std::shared_ptr<Model> cur = std::make_shared<Model>(parent, transform);

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    cur->addMesh(std::make_shared<Mesh>(mesh));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    cur->addSubmodel(createModel(scene, cur, node->mChildren[i]));
  }
  return cur;
}

std::shared_ptr<Model> Model::loadModel(const std::string path) {
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
  auto p = std::filesystem::path(path);

  return createModel(scene, nullptr, scene->mRootNode);
}

void Model::addChild(Model &model) {
  submodels.push_back(std::shared_ptr<Model>(&model));
}

void Model::addMesh(std::shared_ptr<Mesh> m) { meshes.push_back(m); }
void Model::addSubmodel(std::shared_ptr<Model> model) {
  submodels.push_back(model);
}

Model::Model(std::shared_ptr<Model> parent, Transform &transform)
    : parent(parent), transform(transform) {};

const std::vector<std::shared_ptr<Mesh>> Model::getMeshes() { return meshes; }
const std::vector<std::shared_ptr<Model>> Model::getSubmodels() {
  return submodels;
}
const Transform &Model::getTransform() { return transform; }
