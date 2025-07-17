#include "glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "render/camera.hh"
#include "render/shader.hh"
#include "render/utils.hh"

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
               const std::string fragmentShaderPath, bool usesMaterial)
    : usesMaterial(usesMaterial) {
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

void Shader::activate(const Camera &camera) {
  FAIL_ON(!shaderProgram, "Cannot link program");
  glUseProgram(shaderProgram);

  glm::mat4 view = camera.transform.getViewMat();
  glm::mat4 proj = camera.getPerspectiveMat();
  glUniformMatrix4fv(loc("view"), 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(loc("projection"), 1, GL_FALSE, &proj[0][0]);
}

Shader::~Shader() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(shaderProgram);
}

unsigned int Shader::loc(const std::string name) const {
  return glGetUniformLocation(shaderProgram, name.c_str());
}
