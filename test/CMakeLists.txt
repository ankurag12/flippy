cmake_minimum_required(VERSION 3.0)

project(flippy-test)
set(TARGET ${PROJECT_NAME})

include_directories(../include)

add_executable(manual_test_imu 
    manual_test_imu.cpp 
    ../src/LSM6DS33.cpp
    ../src/${HW_IMPL_SRC})
target_link_libraries(manual_test_imu wiringPi pigpiod_if2 rt)

add_executable(manual_test_motor 
    manual_test_motor.cpp 
    ../src/Motor.cpp 
    ../src/Encoder.cpp
    ../src/${HW_IMPL_SRC})
target_link_libraries(manual_test_motor wiringPi pigpiod_if2 rt)

add_executable(manual_test_controller 
    manual_test_controller.cpp 
    ../src/Motor.cpp 
    ../src/Encoder.cpp
    ../src/Controller.cpp
    ../src/LSM6DS33.cpp
    ../src/ConfigReader.cpp
    ../src/${HW_IMPL_SRC})
target_link_libraries(manual_test_controller wiringPi pigpiod_if2 rt)
