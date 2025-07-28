#include "interface.hh"
#include <cstring>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <memory>
#include "commands/command_manager.hh"
#include "commands/select.hh"
#include "interface/console.hh"
#include "render/scene.hh"

static void drawHierarchy(Scene &scene) {
  // Recursive tree drawing function
  std::function<void(const std::shared_ptr<Object> &obj)> DrawNode =
      [&](const std::shared_ptr<Object> &obj) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                                   ImGuiTreeNodeFlags_SpanAvailWidth |
                                   ImGuiTreeNodeFlags_DrawLinesToNodes |
                                   ImGuiTreeNodeFlags_DefaultOpen;
        if (obj.get() == scene.highlightedObject)
          flags |= ImGuiTreeNodeFlags_Selected;
        bool isLeaf = obj->getChildren().empty();
        if (isLeaf)
          flags |=
              ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        bool is_open = ImGui::TreeNodeEx(obj->name.c_str(), flags);

        if (ImGui::BeginItemTooltip()) {
          auto b = obj->getBounds();
          ImGui::SeparatorText("Bounds");
          ImGui::InputFloat3("Min", &b.min.x, "%.3f",
                             ImGuiInputTextFlags_ReadOnly);
          ImGui::InputFloat3("Max", &b.max.x, "%.3f",
                             ImGuiInputTextFlags_ReadOnly);
          ImGui::EndTooltip();
        }

        if (ImGui::IsItemClicked() || ImGui::IsItemActivated())
          scene.highlightedObject = obj.get();

        if (is_open && !isLeaf) {
          for (auto &child : obj->getChildren()) {
            DrawNode(child);
          }
          ImGui::TreePop();
        }
      };

  ImGui::Begin("Hierarchy Browser");
  DrawNode(scene.rootObject);
  ImGui::End();
}

Interface::Interface(Scene &scene, GLFWwindow *window, int width, int height)
    : manager(scene), console(manager), renderSize(width, height) {
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

  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  manager.registerCommand<Select>();
}

void Interface::destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Interface::update(Scene &scene, GLuint renderedTexture) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

  ImGui::Begin("GameWindow", NULL,
               ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration |
                   ImGuiWindowFlags_NoBringToFrontOnFocus);
  ImGui::BeginChild("GameRender");
  ImVec2 wsize = ImGui::GetContentRegionAvail();
  renderSize.x = wsize.x;
  renderSize.y = wsize.y;
  ImGui::Image(renderedTexture, wsize, ImVec2(0, 1), ImVec2(1, 0));
  ImGui::EndChild();
  ImGui::End();

  ImGui::ShowMetricsWindow();

  ImGui::ShowDemoWindow();
  drawHierarchy(scene);

  console.Draw("Console");

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
