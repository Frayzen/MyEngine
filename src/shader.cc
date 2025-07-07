#include "shader.hh"
#include "glad/glad.h"
#include "utils.hh"
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
  const char *shaderSource = shaderCode.c_str();
  glShaderSource(shader, 1, &shaderSource, nullptr);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED " << path << "\n "
              << infoLog << std::endl;
    exit(1);
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

  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER_PROGRAM::COMPILATION_FAILED " << "\n "
              << infoLog << std::endl;
    exit(1);
  }
  GL_ERR;
}

void Shader::activate() {
  FAIL_ON(!shaderProgram, "Cannot link program");
  glUseProgram(shaderProgram);
}

Shader::~Shader() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(shaderProgram);
}

unsigned int Shader::loc(const std::string name) const {
  return glGetUniformLocation(shaderProgram, name.c_str());
}
