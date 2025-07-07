#include "window.hh"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "scene.hh"
#include "utils.hh"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

// Error callback function
static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

int Window::setupWindow(int width, int height, const char *title) {
  glfwSetErrorCallback(glfw_error_callback);

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window)
    return 1;
  glfwMakeContextCurrent(window);

  // NO OPENGL CODE BEFORE THIS
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

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
            std::cerr << "OpenGL Debug: " << message << std::endl;
            // exit(1);
          }
        },
        nullptr);
  }
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(
      window, true); // Second param install_callback=true will install
                     // GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();

  return 0;
}

void Window::run(Scene &scene) {
  double previousTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow(); // Show demo window! :)
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.update();
    scene.render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    double currentTime = glfwGetTime();
    glm::mat4 rotationMatrix =
        glm::rotate(glm::mat4(1.0f), glm::radians((float)currentTime * 30),
                    glm::vec3(0.0f, 1.0f, 0.0f));
    scene.rootObject->transform.rotation = glm::quat_cast(rotationMatrix);

    double deltaTime = currentTime - previousTime;
    (void)deltaTime;
    // std::cout << "dt " << deltaTime << std::endl;
    previousTime = currentTime;

    glfwSwapBuffers(window);
    GL_ERR;
  }
}

Window::~Window() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  if (glfwGetCurrentContext() != nullptr)
    glfwDestroyWindow(glfwGetCurrentContext());
  glfwTerminate();
}
