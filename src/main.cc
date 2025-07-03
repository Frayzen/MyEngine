#include "window.hh"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
  std::cout << "Hello World !" << std::endl;

  MEWindow window;
  if (window.setupWindow(800, 400, "Test"))
    return 1;
  window.run();

  return 0;
}
