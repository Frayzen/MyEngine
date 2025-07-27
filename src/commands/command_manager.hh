#pragma once

#include <vector>
#include "render/scene.hh"

class Command;
class CommandManager {
private:
  std::vector<std::shared_ptr<Command>> commands;
  Scene &scene;

public:
  CommandManager(Scene &scene);
  int execute(std::string cmdLine);
  template <typename T> void registerCommand() {
    auto cmd = std::make_shared<T>();
    commands.push_back(cmd);
    cmd->setup(&scene);
  };
  std::vector<std::string> getCompletions(const std::string &partial);
  void attachScene(Scene *scene);
  Scene &getScene();
};
