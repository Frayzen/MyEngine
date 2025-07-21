#include "render/interface.hh"
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <tcl.h>
#include "render/scene.hh"

Tcl_Interp *interp = Tcl_CreateInterp();
static void InitTcl() {

  Tcl_Init(interp);

  // Register a C++ function
  Tcl_CreateObjCommand(
      interp, "mycommand",
      [](ClientData clientData, Tcl_Interp *interp, int objc,
         Tcl_Obj *const objv[]) -> int {
        (void)objv;
        (void)interp;
        (void)clientData;
        (void)objc;
        std::cout << "OK" << std::endl;
        return TCL_OK;
      },
      nullptr, nullptr);
}

// Get all registered Tcl commands for autocomplete
std::vector<std::string> GetTclCompletions(const std::string &partial) {
  std::vector<std::string> matches;
  Tcl_Obj *cmdObj = Tcl_NewStringObj("info", -1);
  Tcl_ListObjAppendElement(interp, cmdObj, Tcl_NewStringObj("commands", -1));
  Tcl_ListObjAppendElement(interp, cmdObj,
                           Tcl_NewStringObj(partial.c_str(), -1));

  if (Tcl_EvalObj(interp, cmdObj) == TCL_OK) {
    int listSize;
    Tcl_Obj **listItems;
    Tcl_ListObjGetElements(interp, Tcl_GetObjResult(interp), &listSize,
                           &listItems);

    for (int i = 0; i < listSize; i++) {
      matches.push_back(Tcl_GetString(listItems[i]));
    }
  }
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
  InitTcl();
}

// Add these global variables near your InputCallback
static size_t currentCandidate = 0;
static bool showCompletionPopup = true;
static std::string currentInputPrefix;

static std::vector<std::string> filteredCandidates;

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
  auto completionCandidates = GetTclCompletions(currentInputPrefix);
  for (const auto &candidate : completionCandidates) {
    if (candidate.find(currentInputPrefix) == 0) {
      filteredCandidates.push_back(candidate);
    }
  }

  if (!filteredCandidates.empty()) {
    // Show popup next frame
    showCompletionPopup = true;
    currentCandidate = 0;

    // If there's only one match, complete it immediately
    if (filteredCandidates.size() == 1 &&
        data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
      data->DeleteChars((int)(word_start - data->Buf),
                        (int)(word_end - word_start));
      data->InsertChars(data->CursorPos, filteredCandidates[0].c_str());
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

void DrawCompletionPopup(char *inputBuffer, size_t bufferSize) {
  if (!showCompletionPopup)
    return;

  // Filter candidates based on current input prefix
  if (filteredCandidates.empty()) {
    showCompletionPopup = false;
    return;
  }

  // Position the popup near the text input
  ImVec2 pos = ImGui::GetItemRectMin();
  pos.y -= filteredCandidates.size() * ImGui::GetItemRectSize().y;

  ImGui::SetNextWindowPos(pos);
  ImGui::SetNextWindowSize(ImVec2(ImGui::GetItemRectSize().x, 0));

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                           ImGuiWindowFlags_HorizontalScrollbar;

  if (ImGui::Begin("##CompletionPopup", &showCompletionPopup, flags)) {
    for (size_t i = 0; i < filteredCandidates.size(); i++) {
      bool isSelected = (i == currentCandidate);
      if (ImGui::Selectable(filteredCandidates[i].c_str(), isSelected)) {
        // User clicked on an item
        strncpy(inputBuffer, filteredCandidates[i].c_str(), bufferSize - 1);
        inputBuffer[bufferSize - 1] = '\0';
        showCompletionPopup = false;
      }

      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
    }
  }
  ImGui::End();

  // Handle keyboard navigation in the popup
  if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) && currentCandidate > 0) {
    currentCandidate--;
  }
  if (ImGui::IsKeyPressed(ImGuiKey_DownArrow) &&
      currentCandidate < filteredCandidates.size() - 1) {
    currentCandidate++;
  }
  if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
    // Apply the current selection
    strncpy(inputBuffer, filteredCandidates[currentCandidate].c_str(),
            bufferSize - 1);
    inputBuffer[bufferSize - 1] = '\0';
    showCompletionPopup = false;
    ImGui::SetKeyboardFocusHere(-1); // Refocus the input
  }
  if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
    showCompletionPopup = false;
  }
}

void Interface::update(Scene &scene) {
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
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 3));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;

  if (ImGui::Begin("CommandLineInput", nullptr, flags)) {
    static char inputBuffer[256] = "";

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

    bool enterPressed = ImGui::InputTextWithHint(
        "##Input", "Enter command...", inputBuffer, IM_ARRAYSIZE(inputBuffer),
        ImGuiInputTextFlags_EnterReturnsTrue |
            ImGuiInputTextFlags_CallbackEdit |
            ImGuiInputTextFlags_CallbackHistory |
            // ImGuiInputTextFlags_CallbackCompletion |
            ImGuiInputTextFlags_NoHorizontalScroll,
        InputCallback);

    if (enterPressed) {
      inputFocusRequest = true;
      if (inputBuffer[0] != '\0') {
        std::cout << ">" << inputBuffer << std::endl;
        int res = Tcl_Eval(interp, inputBuffer);
        if (res != TCL_OK) {
          std::cerr << "Tcl Error:\n"
                    << "  Message: " << Tcl_GetStringResult(interp) << "\n"
                    << "  Stack: "
                    << Tcl_GetVar(interp, "errorInfo", TCL_GLOBAL_ONLY) << "\n";
        }

        ImGui::SetKeyboardFocusHere(-1);
        inputBuffer[0] = '\0';
      }
    }

    // Draw the completion popup if needed
    // DrawCompletionPopup(inputBuffer, IM_ARRAYSIZE(inputBuffer));

    ImGui::PopStyleColor(2);

    if (inputFocusRequest) {
      ImGui::SetKeyboardFocusHere(-1);
      inputFocusRequest = false;
    }
  }
  ImGui::End();
  ImGui::PopStyleVar(3);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
