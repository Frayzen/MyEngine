#pragma once

#include <string>
#include "commands/command.hh"

class Select : public Command {

  int execute(CommandManager &manager, std::vector<std::string> args);

public:
  Select();
  ~Select() = default;
};
