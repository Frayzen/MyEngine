#pragma once

#include <assimp/mesh.h>
#include <glm/ext/vector_uint3.hpp>
#include <memory>
#include <vector>
#include "render/camera.hh"
#include "render/material.hh"
#include "render/vertex.hh"

class Mesh {
public:
  Mesh() = default;
  Mesh(const aiMesh *mesh);
  void setVertices(std::vector<Vertex> vertices, std::vector<glm::uvec3> tris);
  bool activate(const Camera &cam);
  unsigned int getAmountTris();

  ~Mesh();

  void setMaterial(std::shared_ptr<Material> mat);

private:
  std::shared_ptr<Material> material;

  unsigned int VBO, VAO, EBO = 0;
  unsigned int amountTris;
};
