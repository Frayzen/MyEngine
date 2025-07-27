#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

// always in the shape  [ V (x,y,z) | VN (x,y,z) | VT (u,v) ]
struct Vertex {
  glm::vec3 v;
  glm::vec3 vn;
  glm::vec2 vt;
};
