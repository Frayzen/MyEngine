#pragma once

#include <string>
class Shader {
public:
  Shader(const std::string vertexShaderPath,
         const std::string fragmentShaderPath);
  ~Shader();
  void activate();

private:
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int shaderProgram;
};
