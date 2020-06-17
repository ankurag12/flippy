#include "Controller.h"
#include <iostream>

int main(int argc, char **argv) {
  Controller controller = Controller();
  if (argc == 1) {
    std::cout << "Should enter type of controller to run. Available "
                 "controllers:"
              << std::endl;
    std::cout << "1. linear_dist" << std::endl << "2. flip" << std::endl;
    return 0;
  }
  std::string arg(argv[1]);
  if (arg == "linear_dist") {
    controller.move_linear_dist(0, 0.18);
  } else if (arg == "flip") {
    controller.flip();
  }
  else {
    std::cout << "Unknown controller argument" << std::endl;
  }
  return 0;
}