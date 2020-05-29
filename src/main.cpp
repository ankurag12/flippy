// #include "motor.h"
#include <thread>
#include <errno.h>
#include <chrono>
#include <iostream>
#include "LSM6DS33.h"
#include "motor.h"

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
    
    
    LSM6DS33 imu = LSM6DS33();
    imu.init(1660, 2, 200, 1660, 245);
    while (true) {
        std::cout << "X accel = " << imu.get_accel_reading(Axis::X) << " Y accel = " << imu.get_accel_reading(Axis::Y) << " Z accel = " << imu.get_accel_reading(Axis::Z) << std::endl;
        std::cout << "X gyro = " << imu.get_gyro_reading(Axis::X) << " Y gyro = " << imu.get_gyro_reading(Axis::Y) << " Z gyro = " << imu.get_gyro_reading(Axis::Z) << std::endl;
        std::this_thread::sleep_for(1000ms);

    }
    return 0;
}
