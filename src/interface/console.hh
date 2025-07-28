#pragma once

#include <cctype>
#include <cstdio>
#include "commands/command_manager.hh"
#include "imgui.h"
class Console {
private:
  std::string inputBuf;

  bool shouldComplete;
  size_t selectedCompletion;
  std::vector<std::string> completions;

  std::vector<std::string> history;
  int historyPos; // -1: new line, 0..History.Size-1 browsing history.
  ImGuiTextFilter filter;
  bool autoScroll;
  bool scrollToBottom;
  CommandManager &cmdManager;

public:
  Console(CommandManager &manager);
  ~Console();

  void Draw(const char *title);
  void ExecCommand();
  int TextEditCallback(ImGuiInputTextCallbackData *data);
};
void ShowConsole();
