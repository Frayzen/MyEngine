#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include "render/shader.hh"
#include "render/transform.hh"
class Camera {
public:
  Camera(float fov, float aspect, float near, float far)
      : shader("./assets/vertex.glsl", "./assets/fragment.glsl"), fov(fov),
        near(near), far(far), aspect(aspect) {};
  glm::mat4 getPerspectiveMat();
  void update();

  Transform transform;
  Shader shader;

private:
  float fov, near, far, aspect;
  float movementSpeed = 1.0;
};
