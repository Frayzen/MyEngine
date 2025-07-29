#pragma once

#include "commands/command.hh"
class Orient : public Command {
public:
  Orient();
  ~Orient() = default;

  int execute(CommandManager &manager, std::vector<std::string> args);
};
