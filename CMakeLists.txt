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

set(HW_IMPL_SRC HwImpl_wiringpi.cpp)

set(SOURCES src/main.cpp 
            src/LSM6DS33.cpp
            src/Motor.cpp
            src/Encoder.cpp
            src/Controller.cpp
            src/${HW_IMPL_SRC})

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} wiringPi pigpiod_if2 rt)

add_subdirectory(test)