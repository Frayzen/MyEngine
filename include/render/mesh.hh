#pragma once

#include <assimp/mesh.h>
#include <glm/ext/vector_uint3.hpp>
#include <memory>
#include <string>
#include <vector>
#include "render/material.hh"
#include "render/vertex.hh"

class Mesh {
public:
  Mesh() = default;
  Mesh(const aiMesh *mesh);
  void setVertices(std::vector<Vertex> vertices, std::vector<glm::uvec3> tris);
  bool activate();
  unsigned int getAmountTris() const;

  ~Mesh();

  void setMaterial(std::shared_ptr<Material> mat);
  std::shared_ptr<Material> material;

  const std::string &getName() const;

private:
  std::string name;
  unsigned int VBO, VAO, EBO = 0;
  unsigned int amountTris;
};
