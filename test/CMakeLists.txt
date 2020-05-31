cmake_minimum_required(VERSION 3.0)

project(flippy-test)
set(TARGET ${PROJECT_NAME})

include_directories(../include)

add_executable(manual_test_imu 
    manual_test_imu.cpp 
    ../src/LSM6DS33.cpp)
target_link_libraries(manual_test_imu wiringPi)

add_executable(manual_test_motor 
    manual_test_motor.cpp 
    ../src/Motor.cpp 
    ../src/Encoder.cpp)
target_link_libraries(manual_test_motor wiringPi)
