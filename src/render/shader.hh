#pragma once

#include <string>

class Camera;

class Shader {
public:
  Shader(const Shader &s) = delete;
  Shader();
  Shader(const std::string vertexShaderPath,
         const std::string fragmentShaderPath, bool usesMaterial);
  ~Shader();
  void use();
  void activate(const Camera &camera);
  unsigned int loc(const std::string name) const;

  const bool usesMaterial;

private:
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int shaderProgram;
};
