#include "select.hh"
#include <iostream>

Select::Select() : Command("select", 0, 1) {}

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
