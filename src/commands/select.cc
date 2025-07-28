#include "select.hh"
#include <iostream>
#include "commands/arg.hh"
#include "commands/command.hh"

Select::Select() : Command("select") {
  positionalParams.push_back(Arg{
      .fullName = "target",
      .desc = "the new focus target",
      .type = OBJECT,
      .defaultValue = "root",
  });
}

int Select::execute(CommandManager &manager, std::vector<std::string> args) {
  Scene &scene = manager.getScene();
  if (args.size() == 1) {
    scene.highlightedObject = nullptr;
    return 0;
  }

  Object *found = nullptr;

  auto searchLambda = [args, &found](Object &o) {
    if (found)
      return 0;
    if (o.name == args[1]) {
      found = &o;
      return 0;
    }
    return 1;
  };

  scene.rootObject->apply(searchLambda);
  if (!found) {
    std::cerr << "Not found : " << args[1] << std::endl;
    return 1;
  }
  scene.highlightedObject = found;
  return 0;
}
