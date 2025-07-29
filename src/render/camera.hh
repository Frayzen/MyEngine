#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include "render/shader.hh"
#include "render/transform.hh"

class Camera {
public:
  Camera(float fov, float aspect, float near, float far)
      : shader("./assets/shaders/basic.vert", "./assets/shaders/basic.frag",
               true),
        aspect(aspect), fov(fov), near(near), far(far) {};
  glm::mat4 getPerspectiveMat() const;

  Transform transform;
  Shader shader;
  float aspect;

private:
  float fov, near, far;
  float movementSpeed = 1.0;
};
