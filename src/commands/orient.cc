#include "orient.hh"
#include "commands/command.hh"

Orient::Orient() : Command("orient") {
  positionalParams.push_back({
      .fullName = "orientation",
      .desc = "An axis (x y z) and an (optional) + or -",
      .type = ARG_ORIENTATION,
  });
};

int Orient::execute(CommandManager &manager, std::vector<std::string> args) {
  auto strDir = args[1];
  glm::vec3 dir;
  switch (strDir[0]) {
  case 'x':
    dir = glm::vec3(1, 0, 0);
    break;

  case 'y':
    dir = glm::vec3(0, 1, 0);
    break;

  case 'z':
    dir = glm::vec3(0, 0, 1);
    break;
  default:
    manager.logger << "Please provide a valid direction" << std::endl;
    return 1;
  }

  switch (strDir[1]) {
  case '-':
    dir *= -1;
    break;
  case '+':
  case '\0':
    break;
  default:
    manager.logger << "Please provide a valid direction" << std::endl;
    return 1;
  }

  scene->updateHighlighted(scene->getHighlighted(), dir);
  return 0;
}
