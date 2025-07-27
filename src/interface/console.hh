#pragma once

#include <cctype>
#include <cstdio>
#include "imgui.h"
struct Console {
  char InputBuf[256];
  ImVector<char *> Items;
  ImVector<const char *> Commands;
  ImVector<char *> History;
  int HistoryPos; // -1: new line, 0..History.Size-1 browsing history.
  ImGuiTextFilter Filter;
  bool AutoScroll;
  bool ScrollToBottom;

public:
  Console();
  ~Console();

  void ClearLog();
  void AddLog(const char *fmt, ...) IM_FMTARGS(2);
  void Draw(const char *title);
  void ExecCommand(const char *command_line);
  int TextEditCallback(ImGuiInputTextCallbackData *data);
};
void ShowConsole();
