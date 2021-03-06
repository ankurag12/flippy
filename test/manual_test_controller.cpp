#include "Controller.h"
#include <iostream>

int main(int argc, char **argv) {
  Controller controller;
  if (argc == 1) {
    std::cout << "Should enter type of controller to run. Available "
                 "controllers:"
              << std::endl;
    std::cout << "1. linear_dist" << std::endl << "2. flip" << std::endl << "3. balance" << std::endl << "4. tilt_angle" << std::endl;
    return 0;
  }
  std::string contrl_type(argv[1]);
  if (contrl_type == "linear_dist") {
    if (argc == 2) {
      controller.move_linear_dist(0, 0.18);
    } else {
      controller.move_linear_dist(0, std::stod(argv[2]));
    }
  } else if (contrl_type == "flip") {
    controller.flip();
  } else if (contrl_type == "balance") {
    if (argc == 2) {
      controller.balance(0.0);
    } else {
      controller.balance(std::stod(argv[2]));
    }
  } else if (contrl_type == "tilt_angle") {
    controller.get_tilt_angle();
  } else {
    std::cout << "Unknown controller argument" << std::endl;
  }
  return 0;
}
