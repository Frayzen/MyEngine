#pragma once

#include <sstream>
#include <vector>
#include "render/scene.hh"

class Command;
struct Log;
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

  std::vector<std::string> logs;  // contains the "to-be-shown" lines of logs
  std::ostringstream logger;      // buffer to push to in order to log things
  void log(const char *fmt, ...); // equivalent to appending to logger
  void processLog(); // process the new logs contained into logger and pushes
                     // them in logs

  void attachScene(Scene *scene);
  Scene &getScene();
};
