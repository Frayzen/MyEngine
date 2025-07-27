#include "command.hh"
#include <iostream>

Command::Command(std::string name, size_t minArgNumber, size_t maxArgNumber)
    : name(name), minArgNumber(minArgNumber), maxArgNumber(maxArgNumber),
      scene(nullptr) {};

std::vector<CmdParameter> Command::getParameters() { return parameters; }
void Command::setup(Scene *scene) { this->scene = scene; }
std::string Command::getName() { return name; }

void Command::printHelp() { std::cout << "HELP !" << std::endl; }
