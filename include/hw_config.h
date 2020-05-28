#ifndef HW_CONFIG_H_
#define HW_CONFIG_H_

// Motor wiring config
struct MotorPins {
    int pwm;
    int gpio;
    int enc_a;
    int enc_b;
};

static const MotorPins mp0 = {.pwm = 18, .gpio = 23, .enc_a = 14, .enc_b = 15};
static const MotorPins mp1 = {.pwm = 19, .gpio = 26, .enc_a = 5, .enc_b = 6};

// MotorPins* get_motor_pin_map(int id);


// IMU config



#endif    // HW_CONFIG_H_
