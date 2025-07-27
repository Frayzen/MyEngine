#include "console.hh"

void ShowConsole() {
  static Console console;
  console.Draw("Example: Console");
}
