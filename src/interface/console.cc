#include "console.hh"
#include "commands/command_manager.hh"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

static std::string trim(std::string &str) {
  str.erase(str.find_last_not_of(' ') + 1); // Suffixing spaces
  str.erase(0, str.find_first_not_of(' ')); // Prefixing spaces
  return str;
}

void Console::Draw(const char *title) {
  ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowFocus();
  if (!ImGui::Begin(title, NULL)) {
    ImGui::End();
    return;
  }

  // TODO: display items starting from the bottom
  if (ImGui::SmallButton("Clear")) {
    cmdManager.logs.clear();
  }
  ImGui::SameLine();
  bool copyToCliboard = ImGui::SmallButton("Copy");
  ImGui::Separator();
  // Options menu
  if (ImGui::BeginPopup("Options")) {
    ImGui::Checkbox("Auto-scroll", &autoScroll);
    ImGui::EndPopup();
  }
  // Options, Filter
  ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_O,
                             ImGuiInputFlags_Tooltip);
  if (ImGui::Button("Options"))
    ImGui::OpenPopup("Options");
  ImGui::SameLine();
  filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
  ImGui::Separator();

  // Reserve enough left-over height for 1 separator + 1 input text
  const float footer_height_to_reserve =
      ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
  if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve),
                        ImGuiChildFlags_NavFlattened,
                        ImGuiWindowFlags_HorizontalScrollbar)) {
    if (ImGui::BeginPopupContextWindow()) {
      if (ImGui::Selectable("Clear"))
        cmdManager.logs.clear();
      ImGui::EndPopup();
    }

    // Display every line as a separate entry so we can change their color or
    // add custom widgets. If you only want raw text you can use
    // ImGui::TextUnformatted(log.begin(), log.end()); NB- if you have
    // thousands of entries this approach may be too inefficient and may
    // require user-side clipping to only process visible items. The clipper
    // will automatically measure the height of your first item and then
    // "seek" to display only items in the visible area.
    // To use the clipper we can replace your standard loop:
    //      for (int i = 0; i < Items.Size; i++)
    //   With:
    //      ImGuiListClipper clipper;
    //      clipper.Begin(Items.Size);
    //      while (clipper.Step())
    //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
    // - That your items are evenly spaced (same height)
    // - That you have cheap random access to your elements (you can access
    // them given their index,
    //   without processing all the ones before)
    // You cannot this code as-is if a filter is active because it breaks the
    // 'cheap random-access' property. We would need random-access on the
    // post-filtered list. A typical application wanting coarse clipping and
    // filtering may want to pre-compute an array of indices or offsets of
    // items that passed the filtering test, recomputing this array when user
    // changes the filter, and appending newly elements as they are inserted.
    // This is left as a task to the user until we can manage to improve this
    // example code! If your items are of variable height:
    // - Split them into same height items would be simpler and facilitate
    // random-seeking into your list.
    // - Consider using manual call to IsRectVisible() and skipping extraneous
    // decoration from your items.
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                        ImVec2(4, 1)); // Tighten spacing
    if (copyToCliboard)
      ImGui::LogToClipboard();
    for (auto log : cmdManager.logs) {
      if (!filter.PassFilter(log.c_str()))
        continue;

      // Normally you would store more information in your item than just a
      // string. (e.g. make Items[] an array of structure, store color/type
      // etc.)
      ImVec4 color;
      bool has_color = false;
      if (log.starts_with("[error]")) {
        color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
        has_color = true;
      } else if (log.starts_with("# ")) {
        color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
        has_color = true;
      }
      if (has_color)
        ImGui::PushStyleColor(ImGuiCol_Text, color);
      ImGui::TextUnformatted(log.c_str());
      if (has_color)
        ImGui::PopStyleColor();
    }
    if (copyToCliboard)
      ImGui::LogFinish();

    // Keep up at the bottom of the scroll region if we were already at the
    // bottom at the beginning of the frame. Using a scrollbar or mouse-wheel
    // will take away from the bottom edge.
    if (scrollToBottom ||
        (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
      ImGui::SetScrollHereY(1.0f);
    scrollToBottom = false;

    ImGui::PopStyleVar();
  }
  ImGui::EndChild();
  ImGui::Separator();

  // Command-line
  bool reclaimFocus = false;
  ImGuiInputTextFlags input_text_flags =
      ImGuiInputTextFlags_EnterReturnsTrue |
      ImGuiInputTextFlags_EscapeClearsAll |
      ImGuiInputTextFlags_CallbackCompletion |
      ImGuiInputTextFlags_CallbackAlways | ImGuiInputTextFlags_CallbackHistory;

  auto TextEditCallbackStub = [](ImGuiInputTextCallbackData *data) {
    Console *console = (Console *)data->UserData;
    return console->TextEditCallback(data);
  };
  if (ImGui::InputText("Input", &inputBuf, input_text_flags,
                       TextEditCallbackStub, (void *)this)) {
    if (!completions.empty() && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
      shouldComplete = true;
    } else {
      inputBuf = trim(inputBuf);
      if (!inputBuf.empty())
        ExecCommand();
      inputBuf = "";
    }
    reclaimFocus = true;
  }

  if (ImGui::IsItemActivated())
    shouldComplete = false;

  // Auto-focus on window apparition
  ImGui::SetItemDefaultFocus();
  if (reclaimFocus)
    ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

  ImGui::SetNextWindowPos(
      ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y),
      ImGuiCond_Always, ImVec2(0, 1));
  ImGui::SetNextWindowBgAlpha(1);
  if (ImGui::IsItemActive()) {
    completions = cmdManager.getCompletions(inputBuf);
    if (!completions.empty()) {
      if (ImGui::BeginChild("##Suggestions", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX |
                                ImGuiChildFlags_AlwaysUseWindowPadding)) {

        for (size_t i = 0; i < completions.size(); i++) {
          auto completion = completions[i];
          if (ImGui::Selectable(completion.c_str(), i == selectedCompletion)) {
            shouldComplete = true;
          }
        }
      }
      ImGui::EndChild();

      if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_N))
        selectedCompletion++;
      if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_P))
        selectedCompletion--;
      selectedCompletion %= completions.size();
    }
  }

  ImGui::End();
}

void Console::ExecCommand() {
  cmdManager.log("> %s\n", inputBuf.c_str());

  // Insert into history. First find match and delete it so it can be pushed
  // to the back. This isn't trying to be smart or optimal.
  historyPos = -1;
  for (int i = history.size() - 1; i >= 0; i--)
    if (history[i] == inputBuf) {
      history.erase(history.begin() + i);
      break;
    }
  history.push_back(inputBuf);
  cmdManager.execute(inputBuf);

  // On command input, we scroll to bottom even if AutoScroll==false
  scrollToBottom = true;
}

int Console::TextEditCallback(ImGuiInputTextCallbackData *data) {

  auto completionRequested =
      shouldComplete ||
      data->EventFlag == ImGuiInputTextFlags_CallbackCompletion;
  if (!completions.empty() && completionRequested) {
    // Locate beginning of current word
    const char *word_end = data->Buf + data->CursorPos;
    const char *word_start = word_end;
    while (word_start > data->Buf) {
      const char c = word_start[-1];
      if (c == ' ')
        break;
      word_start--;
    }

    data->DeleteChars((int)(word_start - data->Buf),
                      (int)(word_end - word_start));
    data->InsertChars(data->CursorPos, completions[selectedCompletion].c_str(),
                      completions[selectedCompletion].c_str() +
                          completions[selectedCompletion].size());
    data->InsertChars(data->CursorPos, " ");
  }
  switch (data->EventFlag) {
  case ImGuiInputTextFlags_CallbackHistory: {
    // Example of HISTORY
    const int prev_history_pos = historyPos;
    if (data->EventKey == ImGuiKey_UpArrow) {
      if (historyPos == -1)
        historyPos = history.size() - 1;
      else if (historyPos > 0)
        historyPos--;
    } else if (data->EventKey == ImGuiKey_DownArrow) {
      if (historyPos != -1)
        if (++historyPos >= (int)history.size())
          historyPos = -1;
    }

    // A better implementation would preserve the data on the current input
    // line along with cursor position.
    if (prev_history_pos != historyPos) {
      const char *history_str =
          (historyPos >= 0) ? history[historyPos].c_str() : "";
      data->DeleteChars(0, data->BufTextLen);
      data->InsertChars(0, history_str);
    }
  }
  }
  return 0;
}

Console::Console(CommandManager &manager)
    : shouldComplete(false), cmdManager(manager) {
  cmdManager.logs.clear();
  inputBuf = "";
  historyPos = -1;
  autoScroll = true;
  scrollToBottom = false;
  cmdManager.logger << "Welcome to MyEngine !" << std::endl;
}
Console::~Console() { cmdManager.logs.clear(); }
