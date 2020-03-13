#include "motor.h"
#include <thread>
#include <errno.h>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

int main() {
    Motor m1 = Motor(0);
    m1.run(0.5);
    std::this_thread::sleep_for(5000ms);
    m1.run(0);
    std::this_thread::sleep_for(1000ms);
    m1.run(-0.5);
    std::this_thread::sleep_for(5000ms);
    m1.run(0);
    
    return 0;
}
