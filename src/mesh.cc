#include "mesh.hh"

#include <cstddef>
#include <glad/glad.h>

#include "vertex.hh"
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>

#define VERTEX_ATTRIB_ARRAY 0 // only a single one for now

Mesh::Mesh(const aiMesh *mesh) : Mesh() {
  std::vector<Vertex> vertices;
  Vertex cur;

  if (!mesh->mNormals)
    std::cout << "/!\\ NO NORMALS FOUND IN " << mesh->mName.C_Str() << '\n';
  // Retrieve vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    auto v = mesh->mVertices[i];
    cur.v = {v.x, v.y, v.z};
    auto norm = mesh->mNormals[i];
    cur.vn = {norm.x, norm.y, norm.z};
    if (mesh->HasTextureCoords(0)) {
      auto vt_vec = mesh->mTextureCoords[0][i];
      cur.vt = {vt_vec.x, vt_vec.y};
    } else
      cur.vt = {};
    vertices.emplace_back(cur);
  }
  setVertices(vertices);
}

void Mesh::setVertices(std::vector<Vertex> buffer) {
  if (VAO)
    glDeleteVertexArrays(1, &VAO);
  if (VBO)
    glDeleteBuffers(1, &VBO);

  // Generate VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Generate VBO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(VBO, buffer.size() * sizeof(Vertex), buffer.data(),
               GL_STATIC_DRAW);

  // Set the overall stride of the buffer (where to find the beginning of each
  // Vertex)
  glVertexAttribPointer(VERTEX_ATTRIB_ARRAY, sizeof(Vertex) / sizeof(float),
                        GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(VERTEX_ATTRIB_ARRAY);
  amountTris = buffer.size();
}

Mesh::~Mesh() {
  glDisableVertexAttribArray(VERTEX_ATTRIB_ARRAY);
  if (VAO)
    glDeleteVertexArrays(1, &VAO);
  if (VBO)
    glDeleteBuffers(1, &VBO);
}
