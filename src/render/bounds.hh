#pragma once

#include <cmath>
#include <glm/ext/vector_float3.hpp>

class Bounds {
public:
  Bounds()
      : min(INFINITY, INFINITY, INFINITY),
        max(-INFINITY, -INFINITY, -INFINITY) {}
  glm::vec3 min;
  glm::vec3 max;
  void expand(const Bounds &bounds);
  void expand(const glm::vec3 &bounds);
  glm::vec3 getCenter() const;
  glm::vec3 getDiagonal() const;
};
