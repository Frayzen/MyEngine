#include "command.hh"
#include <iostream>
#include <sstream>

Command::Command(std::string name)
    : positionalParams(), scene(nullptr), name(name) {};

void Command::setup(Scene *scene) { this->scene = scene; }
std::string Command::getName() { return name; }

std::string Command::getHelpString() const {
  std::ostringstream help;
  help << "[Help] " << name << std::endl;
  help << name;

  for (auto arg : positionalParams)
    help << " <" << arg.fullName << ">";

  help << std::endl;
  for (auto arg : positionalParams) {
    help << arg.fullName << ": " << arg.desc;
    if (arg.defaultValue != nullptr)
      help << "(default = " << arg.defaultValue << ")";
    help << std::endl;
  }
  return help.str();
}

const std::vector<Arg> Command::getPositionalParams() const {
  return positionalParams;
}
