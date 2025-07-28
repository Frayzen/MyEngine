#include "bounds.hh"
#include <glm/common.hpp>
#include <glm/ext/vector_float3.hpp>

void Bounds::expand(const Bounds &bounds) {
  max = glm::max(bounds.max, max);
  min = glm::min(bounds.min, min);
}

void Bounds::expand(const glm::vec3 &v) {
  max = glm::max(v, max);
  min = glm::min(v, min);
}
glm::vec3 Bounds::getCenter() const { return (min + max) / 2.0f; }
glm::vec3 Bounds::getDiagonal() const { return max - min; }
