#include "render/mesh.hh"

#include <glad/glad.h>
#include <cmath>
#include <cstddef>

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/common.hpp>
#include <glm/ext/vector_uint3.hpp>
#include <iostream>
#include <vector>
#include "render/bounds.hh"
#include "render/utils.hh"
#include "render/vertex.hh"

Mesh::Mesh(const aiMesh *mesh) : Mesh() {
  name = mesh->mName.C_Str();
  std::vector<Vertex> vertices;
  std::vector<glm::uvec3> tris;
  Vertex cur;

  Bounds bounds;

  // Retrieve vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    // V
    auto v = mesh->mVertices[i];
    cur.v = {v.x, v.y, v.z};

    bounds.expand(cur.v);

    // VN
    if (!mesh->mNormals) {
      cur.vn = {0, 0, 0}; // Default normal if missing
    } else {
      auto norm = mesh->mNormals[i];
      cur.vn = {norm.x, norm.y, norm.z};
    }

    // VT
    if (mesh->HasTextureCoords(0)) {
      auto vt_vec = mesh->mTextureCoords[0][i];
      cur.vt = {vt_vec.x, vt_vec.y};
    } else
      cur.vt = {};

    vertices.emplace_back(cur);
  }

  glm::uvec3 tri;
  for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    auto face = mesh->mFaces[i];
    if (face.mNumIndices == 3) {
      tri.x = face.mIndices[0];
      tri.y = face.mIndices[1];
      tri.z = face.mIndices[2];
      tris.push_back(tri);
    }
  }

  setVertices(vertices, tris, bounds);
}

void Mesh::setVertices(std::vector<Vertex> vertices,
                       std::vector<glm::uvec3> tris, const Bounds &bounds) {
  this->bounds = bounds;
  if (vertices.empty() || tris.empty()) {
    std::cerr << "Error: Empty vertex or index data!" << std::endl;
    VAO = 0;
    return;
  }
  for (const auto &tri : tris) {
    if (tri.x >= vertices.size() || tri.y >= vertices.size() ||
        tri.z >= vertices.size()) {
      std::cerr << "Error: Index out of bounds!" << std::endl;
      return;
    }
  }
  assert(sizeof(Vertex) == sizeof(float) * 8);

  if (VAO || VBO) {
    std::cerr << "Cannot assign vertices twice" << std::endl;
    exit(1);
  }

  // Generate VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Generate VBO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  // Generate EBO
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(glm::uvec3),
               tris.data(), GL_STATIC_DRAW);

  // Set the overall stride of the buffer (where to find the beginning of each
  // Vertex)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, v));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, vn));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, vt));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  amountTris = tris.size();
  assert(amountTris != 0);

  glBindVertexArray(0);
  GL_ERR;
}

Mesh::~Mesh() {
  glBindVertexArray(VAO);
  if (VBO)
    glDeleteBuffers(1, &VBO);
  if (EBO)
    glDeleteBuffers(1, &EBO);
  glBindVertexArray(0);
  if (VAO)
    glDeleteVertexArrays(1, &VAO);
}

bool Mesh::activate() {
  if (VAO == 0)
    return false;
  glBindVertexArray(VAO);
  GL_ERR;
  return true;
}

unsigned int Mesh::getAmountTris() const { return amountTris; }

void Mesh::setMaterial(std::shared_ptr<Material> mat) { material = mat; }

const std::string &Mesh::getName() const { return name; }
const Bounds &Mesh::getBounds() const { return bounds; }
