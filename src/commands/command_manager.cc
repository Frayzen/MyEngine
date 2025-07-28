#include "command_manager.hh"
#include <iostream>
#include "commands/command.hh"
#include "render/scene.hh"

CommandManager::CommandManager(Scene &scene) : scene(scene) {}

static std::vector<std::string> convertCommandLine(std::string input) {
  std::istringstream iss(input);
  std::vector<std::string> args;
  std::string arg;

  // Split the input string into individual arguments
  while (iss >> arg) {
    args.push_back(arg);
  }

  if (input.ends_with(' '))
    args.push_back("");

  return args;
}

int CommandManager::execute(std::string cmdLine) {
  auto args = convertCommandLine(cmdLine);
  if (args.empty())
    return 1;
  for (auto cmd : commands) {
    if (cmd->getName() == args[0]) {
      if (cmd->minArgNumber + 1 > args.size() ||
          cmd->maxArgNumber + 1 < args.size()) {
        cmd->printHelp();
        return 1;
      }
      cmd->execute(*this, args);
      return 0;
    }
  }
  return 1;
}

std::vector<std::string>
CommandManager::getCompletions(const std::string &partial) {
  std::vector<std::string> matches;
  auto args = convertCommandLine(partial);
  if (args.empty())
    return matches;
  if (args.size() == 1) {
    for (auto cmd : commands) {
      if (cmd->name.starts_with(args[0]))
        matches.push_back(cmd->name);
    }
    matches.push_back("selact");
  } else {
    matches.push_back("args");
  }
  return matches;
}

void CommandManager::attachScene(Scene *scene) {
  for (auto cmd : commands)
    cmd->setup(scene);
}

Scene &CommandManager::getScene() { return scene; }
