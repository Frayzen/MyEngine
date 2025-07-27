#pragma once

#include <string>
#include <vector>
#include "commands/command_manager.hh"
#include "render/scene.hh"

enum ArgType { VOID, STR, OBJ_NAME };

struct CmdParameter {
  std::string fullName;
  std::string desc;
  char shortName;
  enum ArgType type;
  std::string defaultValue = "";
};

class Command {

protected:
  std::string name;
  size_t minArgNumber;
  size_t maxArgNumber;
  std::vector<CmdParameter> parameters;
  Scene *scene;

public:
  Command(std::string name, size_t minArgNumber, size_t maxArgNumber);
  void setup(Scene *scene);

  std::vector<CmdParameter> getParameters();
  virtual int execute(CommandManager &manager,
                      std::vector<std::string> args) = 0;
  virtual ~Command() = default;
  std::string getName();
  friend class CommandManager;
  void printHelp();
};
