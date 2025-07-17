#include "render/interface.hh"
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "render/scene.hh"

static void drawHierarchy(const Scene &scene) {
  // Recursive tree drawing function
  std::function<void(const std::shared_ptr<Object> &obj)> DrawNode =
      [&](const std::shared_ptr<Object> &obj) {
        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool isLeaf = obj->children.empty();
        if (isLeaf)
          flags |=
              ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        bool is_open = ImGui::TreeNodeEx(obj->name.c_str(), flags);

        // Optional: Add folder/file icons (need an icon font)
        // ImGui::SameLine();
        // ImGui::Text(node.is_folder ? ICON_FA_FOLDER : ICON_FA_FILE);

        if (is_open && !isLeaf) {
          for (const auto &child : obj->children) {
            DrawNode(child);
          }
          ImGui::TreePop();
        }
      };

  ImGui::Begin("Hierarchy Browser");
  DrawNode(scene.rootObject);
  ImGui::End();
}

Interface::Interface(GLFWwindow *window) {
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
}

void Interface::update(const Scene &scene) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  drawHierarchy(scene);

  // Calculate position and size
  const float input_height = ImGui::GetFrameHeightWithSpacing();
  const ImVec2 display_size = ImGui::GetIO().DisplaySize;
  const ImVec2 window_pos(0, display_size.y - input_height);
  const ImVec2 window_size(display_size.x, input_height);

  // Set up window
  ImGui::SetNextWindowPos(window_pos);
  ImGui::SetNextWindowSize(window_size);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
                      ImVec2(5, 3)); // Small padding
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;

  if (ImGui::Begin("CommandLineInput", nullptr, flags)) {
    static char inputBuffer[256] = "";

    // Style the input to be minimal but visible
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

    // Full-width input with slight padding
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

    // Use InputTextWithHint for a cleaner look
    bool enterPressed = ImGui::InputTextWithHint(
        "##Input", "Enter command...", inputBuffer, IM_ARRAYSIZE(inputBuffer),
        ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::PopStyleColor(2);

    if (enterPressed && inputBuffer[0] != '\0') {
      std::cout << "Command entered: " << inputBuffer << std::endl;
      inputBuffer[0] = '\0';
    }
    // Auto-focus
    ImGui::SetKeyboardFocusHere(-1);
  }
  ImGui::End();
  ImGui::PopStyleVar(3);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Interface::destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
