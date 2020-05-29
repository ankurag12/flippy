cmake_minimum_required(VERSION 3.0)

project(flippy)
set(TARGET ${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD 14)

message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message("CMAKE_HOME_DIRECTORY: ${CMAKE_HOME_DIRECTORY}")
message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")

include_directories(include)

set(SOURCES src/main.cpp 
            src/LSM6DS33.cpp
            src/motor.cpp)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} wiringPi)

add_executable(manual_test_imu test/manual_test_imu.cpp src/LSM6DS33.cpp)
target_link_libraries(manual_test_imu wiringPi)

add_executable(manual_test_motor test/manual_test_motor.cpp src/motor.cpp)
target_link_libraries(manual_test_motor wiringPi)
