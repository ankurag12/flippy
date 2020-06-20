#include "Motor.h"
#include <chrono>
#include <errno.h>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {

  Motor left_motor(MotorSide::LEFT);
  std::cout << "Left motor tick count = " << left_motor.get_current_tick_count()
            << std::endl;
  left_motor.run(0.2);
  std::this_thread::sleep_for(2000ms);
  left_motor.run(0);
  std::cout << "Left motor tick count = " << left_motor.get_current_tick_count()
            << std::endl;

  std::this_thread::sleep_for(1000ms);
  left_motor.run(-0.2);
  std::this_thread::sleep_for(2000ms);
  left_motor.run(0);
  std::cout << "Left motor tick count = " << left_motor.get_current_tick_count()
            << std::endl;

  Motor right_motor(MotorSide::RIGHT);
  std::cout << "Right motor tick count = "
            << right_motor.get_current_tick_count() << std::endl;

  right_motor.run(0.2);
  std::this_thread::sleep_for(2000ms);
  right_motor.run(0);
  std::cout << "Right motor tick count = "
            << right_motor.get_current_tick_count() << std::endl;

  std::this_thread::sleep_for(1000ms);
  right_motor.run(-0.2);
  std::this_thread::sleep_for(2000ms);
  right_motor.run(0);
  std::cout << "Right motor tick count = "
            << right_motor.get_current_tick_count() << std::endl;

  return 0;
}
