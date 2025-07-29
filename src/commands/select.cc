#include "select.hh"
#include <iostream>
#include "commands/arg.hh"
#include "commands/command.hh"

Select::Select() : Command("select") {
  positionalParams.push_back(Arg{
      .fullName = "target",
      .desc = "the new focus target",
      .type = ARG_SCENE_OBJECT,
      .defaultValue = "root",
  });
}

int Select::execute(CommandManager &manager, std::vector<std::string> args) {
  Scene &scene = manager.getScene();
  Object *found = nullptr;

  auto searchLambda = [args, &found](Object &o) {
    if (found)
      return false;
    if (o.name == args[1]) {
      found = &o;
      return false;
    }
    return true;
  };

  scene.rootObject->apply(searchLambda);
  if (!found) {
    std::cerr << "Not found : " << args[1] << std::endl;
    return 1;
  }
  scene.updateHighlighted(found);
  return 0;
}
