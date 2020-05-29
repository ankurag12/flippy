#include "motor.h"
#include <chrono>
#include <errno.h>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {
  Motor left_motor = Motor(MotorSide::LEFT);
  left_motor.run(0.5);
  std::this_thread::sleep_for(5000ms);
  left_motor.run(0);
  std::this_thread::sleep_for(1000ms);
  left_motor.run(-0.5);
  std::this_thread::sleep_for(5000ms);
  left_motor.run(0);

  Motor right_motor = Motor(MotorSide::RIGHT);
  right_motor.run(0.5);
  std::this_thread::sleep_for(5000ms);
  right_motor.run(0);
  std::this_thread::sleep_for(1000ms);
  right_motor.run(-0.5);
  std::this_thread::sleep_for(5000ms);
  right_motor.run(0);
  return 0;
}
