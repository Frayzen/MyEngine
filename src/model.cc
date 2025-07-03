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
#include <cstddef>
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
  transform.rotation = {rot.x, rot.y, rot.z, rot.w};
  return transform;
}

static std::shared_ptr<Model> createModel(std::shared_ptr<Model> parent,
                                          const aiNode *node) {
  Transform transform = getTransform(node);
  std::shared_ptr<Model> cur = std::make_shared<Model>(parent, transform);

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    cur->addMesh(std::make_shared<Mesh>(node->mMeshes[i]));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    createModel(cur, node->mChildren[i]);
  }
  return cur;
}

Model Model::loadModel(std::string &path) {
  static Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path.c_str(),
      aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
          aiProcess_GenUVCoords | aiProcess_FlipUVs |
          aiProcess_RemoveRedundantMaterials |
          aiProcess_GenSmoothNormals /* or aiProcess_GenNormals */);
  FAIL_ON(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
              !scene->mRootNode,
          "The mesh " << path.c_str() << "could not be loaded.\nReason:"
                      << importer.GetErrorString());
  auto p = std::filesystem::path(path);

  return *createModel(NULL, scene->mRootNode);
}

void Model::addChild(Model &model) {
  submodels.push_back(std::shared_ptr<Model>(&model));
}

void Model::addMesh(std::shared_ptr<Mesh> m) { meshes.push_back(m); }

Model::Model(std::shared_ptr<Model> parent, Transform &transform)
    : parent(parent), transform(transform) {};
