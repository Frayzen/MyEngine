#include "glad/glad.h"
#include "render/window.hh"

#include <GL/glext.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include "render/interface.hh"
#include "render/scene.hh"
#include "render/utils.hh"

// Error callback function
static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
  while (1)
    ;
}

static void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

void Window::setupFrameBuffer(int width, int height) {
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  // generate texture
  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  // render buffer
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);
  // attach it to currently bound framebuffer object
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         textureColorbuffer, 0);
  FAIL_ON(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE,
          "Failed to create framebuffer");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Window::Window(int width, int height, const char *title) {
  glfwSetErrorCallback(glfw_error_callback);

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  glfwWindowHint(GLFW_DEPTH_BITS, 24); // Request depth buffer

  glfwWindowHint(GLFW_SAMPLES, 4);

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  FAIL_ON(!window, "Could not create glfw window.");
  glfwMakeContextCurrent(window);

  // NO OPENGL CODE BEFORE THIS
  FAIL_ON(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
          "Failed to initialize GLAD");

  if (GLAD_GL_KHR_debug) {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(
        [](GLenum source, GLenum type, GLuint id, GLenum severity,
           GLsizei length, const GLchar *message, const void *userParam) {
          if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {

            (void)source;
            (void)type;
            (void)id;
            (void)severity;
            (void)length;
            (void)length;
            (void)message;
            (void)userParam;
            if (severity == GL_DEBUG_SEVERITY_HIGH)
              std::cerr << "OpenGL Error: " << message << std::endl;
            // while (1)
            //   ;
          }
        },
        nullptr);
  }
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glViewport(0, 0, width, height);

  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  interface = std::make_shared<Interface>(window);

  setupFrameBuffer(width, height);
  screenShader = std::make_shared<Shader>(
      "./assets/shaders/screen.vert", "./assets/shaders/screen.frag", false);
}

void Window::run(Scene &scene) {
  double previousTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glDepthMask(GL_TRUE);

    // first pass
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    scene.update();
    scene.render();

    // second pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    screenShader->use();

    static GLuint quadVAO = 0;
    if (!quadVAO) {
      // Generate VAO
      glGenVertexArrays(1, &quadVAO);
      glBindVertexArray(quadVAO);

      unsigned int VBO;
      // Generate VBO
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      const float vertices[] = {
          // positions   // texCoords
          -1.0f, 1.0f, 0.0f,  1.0f,  1.0f, -1.0f,
          1.0f,  0.0f, -1.0f, -1.0f, 0.0f, 0.0f,

          -1.0f, 1.0f, 0.0f,  1.0f,  1.0f, 1.0f,
          1.0f,  1.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
      };
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      // Position attribute
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void *)0);

      // Texture coordinate attribute
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void *)(2 * sizeof(float)));

      glBindVertexArray(0); // Unbind VAO
    }

    glBindVertexArray(quadVAO);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glUniform1i(screenShader->loc("screenTexture"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    interface->update(scene);

    double currentTime = glfwGetTime();
    // glm::mat4 rotationMatrix =
    //     glm::rotate(glm::mat4(1.0f), glm::radians((float)currentTime * 30),
    //                 glm::vec3(0.0f, 1.0f, 0.0f));
    // scene.rootObject->transform.rotation = glm::quat_cast(rotationMatrix);

    double deltaTime = currentTime - previousTime;
    (void)deltaTime;
    // std::cout << "dt " << deltaTime << std::endl;
    previousTime = currentTime;

    glfwSwapBuffers(window);
    GL_ERR;
  }
}

Window::~Window() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &framebuffer);
  interface->destroy();
  if (glfwGetCurrentContext() != nullptr)
    glfwDestroyWindow(glfwGetCurrentContext());
  glfwTerminate();
}
