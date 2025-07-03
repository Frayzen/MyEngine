#include "window.hh"
#include "glad/glad.h"
#include "scene.hh"
#include <iostream>

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

int Window::setupWindow(int width, int height, const char *title) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window)
    return 1;
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);
  return 0;
}

void Window::run(Scene &scene) {
  double previousTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.update();
    scene.render();
    // Check for OpenGL errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
      std::cerr << "OpenGL Error: " << err << std::endl;
    }

    double currentTime = glfwGetTime();
    double deltaTime = currentTime - previousTime;
    std::cout << "dt " << deltaTime << std::endl;
    previousTime = currentTime;

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}

Window::~Window() {
  glfwTerminate();
  if (glfwGetCurrentContext() != nullptr)
    glfwDestroyWindow(glfwGetCurrentContext());
}
