#include <iostream>
#include <map>
#include "pin_config.h"

static const MotorPins mp0 = {.pwm = 18, .gpio = 23, .enc_a = 14, .enc_b = 15};
static const MotorPins mp1 = {.pwm = 19, .gpio = 26, .enc_a = 5, .enc_b = 6};

std::map<int, MotorPins> motor_pins_map = {{0, mp0}, {1, mp1}};

MotorPins* get_motor_pin_map(int id) {
    if (motor_pins_map.count(id) == 0) {
        std::cerr << "Invalid motor ID " << id << std::endl;
        return nullptr;
    }
    return &motor_pins_map[id];
    
}

