#include "shader.hh"
#include "glad/glad.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>

static std::string readFile(const std::string path) {
  std::ifstream t(path);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

static unsigned int createShader(const std::string path, GLenum shaderType) {
  unsigned int shader = glCreateShader(shaderType);
  std::string shaderCode = readFile(path);
  glShaderSource(shader, 1, (const GLchar *const *)path.c_str(), NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    return 0;
  }
  return shader;
}

Shader::Shader(const std::string vertexShaderPath,
               const std::string fragmentShaderPath) {
  vertexShader = createShader(vertexShaderPath, GL_VERTEX_SHADER);
  fragmentShader = createShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
}

void Shader::activate() { glUseProgram(shaderProgram); }

Shader::~Shader() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(shaderProgram);
}
