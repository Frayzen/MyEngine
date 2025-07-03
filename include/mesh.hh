#pragma once

#include "vertex.hh"
#include <assimp/mesh.h>
#include <vector>

class Mesh {
public:
  Mesh() {};
  Mesh(const aiMesh *mesh);
  void setVertices(std::vector<Vertex> buffer);

  ~Mesh();

private:
  unsigned int VBO, VAO;
  unsigned int amountTris;
};
