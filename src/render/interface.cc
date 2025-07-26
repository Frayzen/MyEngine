#include "render/interface.hh"
#include <algorithm>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "render/scene.hh"

static std::string logs = "";

// Get all registered Tcl commands for autocomplete
std::vector<std::string> GetCompletions(const std::string &partial) {
  std::vector<std::string> matches;
  if (partial.empty())
    return matches;
  const std::string tst = "ping";
  if (tst.starts_with(partial))
    matches.push_back(tst);
  return matches;
}

static bool inputFocusRequest = true;

static void drawHierarchy(Scene &scene) {
  // Recursive tree drawing function
  std::function<void(std::shared_ptr<Object> & obj)> DrawNode =
      [&](std::shared_ptr<Object> &obj) {
        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (obj == scene.highlightedObject)
          flags |= ImGuiTreeNodeFlags_Selected;
        bool isLeaf = obj->children.empty();
        if (isLeaf)
          flags |=
              ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        bool is_open = ImGui::TreeNodeEx(obj->name.c_str(), flags);
        if (ImGui::IsItemClicked())
          scene.highlightedObject = obj;

        // Optional: Add folder/file icons (need an icon font)
        // ImGui::SameLine();
        // ImGui::Text(node.is_folder ? ICON_FA_FOLDER : ICON_FA_FILE);

        if (is_open && !isLeaf) {
          for (auto &child : obj->children) {
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

  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

// Add these global variables near your InputCallback
static size_t currentCandidate = 0;
static bool openAutocomplete = true;
static std::string currentInputPrefix;

static std::vector<std::string> autocompleteCandidates;

int InputCallback(ImGuiInputTextCallbackData *data) {
  // Get current word prefix
  const char *word_end = data->Buf + data->CursorPos;
  const char *word_start = word_end;
  while (word_start > data->Buf) {
    const char c = word_start[-1];
    if (c == ' ' || c == '\t' || c == ',' || c == ';')
      break;
    word_start--;
  }

  currentInputPrefix = std::string(word_start, word_end);

  // Filter candidates based on prefix
  auto completionCandidates = GetCompletions(currentInputPrefix);
  autocompleteCandidates.clear();
  for (const auto &candidate : completionCandidates) {
    if (candidate.find(currentInputPrefix) == 0) {
      autocompleteCandidates.push_back(candidate);
    }
  }

  if (!autocompleteCandidates.empty()) {
    // Show popup next frame
    openAutocomplete = true;
    currentCandidate = 0;

    // If there's only one match, complete it immediately
    if (autocompleteCandidates.size() == 1 &&
        data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
      data->DeleteChars((int)(word_start - data->Buf),
                        (int)(word_end - word_start));
      data->InsertChars(data->CursorPos, autocompleteCandidates[0].c_str());
    }
  }
  inputFocusRequest = true;
  return 0;
}

void Interface::destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Interface::update(Scene &scene) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowMetricsWindow();

  const ImVec2 display_size = ImGui::GetIO().DisplaySize;

  drawHierarchy(scene);

  if (openAutocomplete) {

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetStyle().WindowBorderSize +
                                       ImGui::GetStyle().FramePadding.x,
                                   display_size.y -
                                       ImGui::GetFrameHeightWithSpacing() -
                                       ImGui::GetStyle().WindowBorderSize),
                            0, ImVec2(0.0f, 1.0f));
    ImGui::SetNextWindowFocus();
    inputFocusRequest = true;
    ImGui::OpenPopup("autocomplete");
    openAutocomplete = false;
  }

  static int autocompleteSelected = 0;
  static std::string insertValue;

  if (!autocompleteCandidates.empty() &&
      ImGui::BeginPopup("autocomplete",
                        ImGuiWindowFlags_NoMove |
                            ImGuiWindowFlags_NoFocusOnAppearing)) {
    if (autocompleteCandidates.size() == 1) {
      insertValue = autocompleteCandidates[0].c_str();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
      ++autocompleteSelected;
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
      --autocompleteSelected;
    if (ImGui::IsKeyPressed(ImGuiKey_Enter))
      insertValue = autocompleteCandidates[autocompleteSelected].c_str();
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
      ImGui::CloseCurrentPopup();
    autocompleteSelected = std::clamp(autocompleteSelected, 0,
                                      (int)autocompleteCandidates.size() - 1);
    for (int i = 0, c = autocompleteCandidates.size(); i < c; ++i) {
      const char *value = autocompleteCandidates[i].c_str();
      if (ImGui::Selectable(value, autocompleteSelected == i)) {
        insertValue = value;
      }
    }
    ImGui::EndPopup();
  }

  ImGui::SetNextWindowPos(ImVec2(0.0f, display_size.y), 0, ImVec2(0.0f, 1.0f));
  ImGui::SetWindowSize(ImVec2(display_size.x, 0));

  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
  ImGui::SetNextWindowSize(
      ImVec2(display_size.x,
             0)); // Width = -1 (fill available space), Height = 0 (auto)
  if (ImGui::Begin("Commands", nullptr, flags)) {
    ImGui::Text("LOGS");
    ImGui::TextColored(ImVec4(0.6, 0.6, 0.6, 1), "%s", logs.c_str());

    // Make input fill available width
    static char inputBuffer[256] = "";

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    bool enterPressed = ImGui::InputTextWithHint(
        "##Input", "Enter command...", inputBuffer, IM_ARRAYSIZE(inputBuffer),
        ImGuiInputTextFlags_EnterReturnsTrue |
            ImGuiInputTextFlags_CallbackEdit |
            ImGuiInputTextFlags_CallbackHistory |
            // ImGuiInputTextFlags_CallbackCompletion |
            ImGuiInputTextFlags_NoHorizontalScroll,
        InputCallback);

    ImGui::SetNextItemWidth(display_size.x);

    if (inputFocusRequest) {
      ImGui::SetKeyboardFocusHere(-1);
      inputFocusRequest = false;
    }
    if (enterPressed) {
      inputFocusRequest = true;
      if (inputBuffer[0] != '\0') {
        std::cout << ">" << inputBuffer << std::endl;
        autocompleteCandidates.clear();
        ImGui::SetKeyboardFocusHere(-1);
        inputBuffer[0] = '\0';
      }
    }

    ImGui::End();
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
