#include "command.hh"
#include <iostream>
#include <sstream>
#include "commands/arg.hh"
#include "render/object.hh"

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

static std::vector<std::string> completeSceneObj(Scene *scene,
                                                 std::string match) {
  std::vector<std::string> proposals;

  auto lambdaFill = [&proposals, match](Object &o) {
    proposals.push_back(o.name);
    return true;
  };
  scene->rootObject->apply(lambdaFill);
  return proposals;
}

const std::vector<std::string>
Command::argumentCompletion(std::vector<std::string> &args) const {
  auto strParams = std::vector<std::string>(args.begin() + 1, args.end());
  if (strParams.size() > positionalParams.size())
    return {};
  std::vector<std::string> candidates;
  switch (positionalParams[strParams.size() - 1].type) {
  case ARG_SCENE_OBJECT:
    candidates = completeSceneObj(scene, strParams.back());
    break;
  case ARG_ORIENTATION:
    candidates = {"x", "y", "z", "x+", "x-", "y+", "y-", "z+", "z-"};
    break;
  default:
    return {};
  }
  std::vector<std::string> matches;
  std::copy_if(candidates.begin(), candidates.end(),
               std::back_inserter(matches), [strParams](std::string &s) {
                 return s.find(strParams.back()) != std::string::npos;
               });
  return matches;
}
