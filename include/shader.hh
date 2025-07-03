#pragma once

#include <string>
class Shader {
public:
  Shader(const std::string vertexShaderPath,
         const std::string fragmentShaderPath);
  ~Shader();
  void activate();
  unsigned int loc(const std::string name);

private:
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int shaderProgram;
};
