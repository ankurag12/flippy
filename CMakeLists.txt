cmake_minimum_required(VERSION 3.0)

project(flippy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD 14)

message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message("CMAKE_HOME_DIRECTORY: ${CMAKE_HOME_DIRECTORY}")
message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")

include_directories(./include)

set(SOURCES ./src/main.cpp 
            ./src/motor.cpp 
            ./src/pin_mapping.cpp)

add_executable(flippy ${SOURCES})

target_link_libraries(flippy wiringPi)
