#include <iostream>
#include <map>
#include "pin_config.h"



std::map<int, MotorPins> motor_pins_map = {{0, mp0}, {1, mp1}};

MotorPins* get_motor_pin_map(int id) {
    if (motor_pins_map.count(id) == 0) {
        std::cerr << "Invalid motor ID " << id << std::endl;
        return nullptr;
    }
    return &motor_pins_map[id];
    
}

