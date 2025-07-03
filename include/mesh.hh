#pragma once

#include "vertex.hh"
#include <assimp/mesh.h>
#include <glm/ext/vector_uint3.hpp>
#include <vector>

class Mesh {
public:
  Mesh() {};
  Mesh(const aiMesh *mesh);
  void setVertices(std::vector<Vertex> vertices, std::vector<glm::uvec3> tris);
  void activate();
  unsigned int getAmountTris();

  ~Mesh();

private:
  unsigned int VBO, VAO, EBO;
  unsigned int amountTris;
};
