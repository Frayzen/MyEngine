#pragma once

#include <string>
#include <vector>
#include "commands/arg.hh"
#include "commands/command_manager.hh"
#include "render/scene.hh"

class Command {

protected:
  std::vector<Arg> positionalParams;
  Scene *scene;

public:
  const std::string name;
  Command(std::string name);
  void setup(Scene *scene);
  virtual int execute(CommandManager &manager,
                      std::vector<std::string> args) = 0;
  virtual ~Command() = default;
  std::string getName();
  friend class CommandManager;
  std::string getHelpString() const;
  const std::vector<Arg> getPositionalParams() const;
};
