#include "command_manager.hh"
#include <algorithm>
#include <cstdarg>
#include <iostream>
#include <string>
#include "commands/command.hh"
#include "imgui.h"
#include "render/scene.hh"

CommandManager::CommandManager(Scene &scene) : scene(scene) {}

static std::vector<std::string> convertCommandLine(std::string input,
                                                   bool endingSpaceIsArg) {
  std::istringstream iss(input);
  std::vector<std::string> args;
  std::string arg;
  // Split the input string into individual arguments
  while (iss >> arg) {
    args.push_back(arg);
  }
  if (endingSpaceIsArg && input.ends_with(' '))
    args.push_back("");
  return args;
}

int CommandManager::execute(std::string cmdLine) {
  auto args = convertCommandLine(cmdLine, false);
  if (args.empty())
    return 1;
  for (auto cmd : commands) {
    if (cmd->getName() == args[0]) {

      auto positionals = cmd->getPositionalParams();

      size_t argNb = args.size() - 1;
      size_t missing = 0;
      if (argNb < positionals.size())
        missing = positionals.size() - argNb;

      int insertArgPos = 1;
      size_t cur = 0;
      while (missing > 0 && cur < positionals.size()) {
        auto pos = positionals[cur++];
        if (pos.defaultValue == nullptr) {
          insertArgPos++;
          continue;
        }
        args.insert(args.begin() + insertArgPos, pos.defaultValue);
        missing--;
      }
      if (missing) {
        logger << cmd->getHelpString();
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
  auto args = convertCommandLine(partial, true);
  if (args.empty())
    return matches;
  static Command *lastCmd;
  if (args.size() == 1) {
    for (auto cmd : commands) {
      if (cmd->name.starts_with(args[0])) {
        matches.push_back(cmd->name);
        lastCmd = cmd.get();
      }
    }
  } else {
    // TODO
    (void)lastCmd;
  }
  return matches;
}

void CommandManager::attachScene(Scene *scene) {
  for (auto cmd : commands)
    cmd->setup(scene);
}

Scene &CommandManager::getScene() { return scene; }

void CommandManager::log(const char *fmt, ...) {
  char buf[1024];
  va_list args;
  va_start(args, fmt);
  std::vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
  buf[IM_ARRAYSIZE(buf) - 1] = 0;
  va_end(args);
  logger << buf;
}

void CommandManager::processLog() {
  std::string line;
  std::istringstream stream(logger.str());
  while (std::getline(stream, line)) {
    logs.push_back(line);
  }
  logger.str("");
}
