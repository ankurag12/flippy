#ifndef MOTOR_H_
#define MOTOR_H_

#include <map>

enum MotorSide { LEFT, RIGHT };

// Motor wiring config
struct MotorPins {
  int pwm;
  int gpio;
  int enc_a;
  int enc_b;
};

// Assuming WiringPi numbering scheme
// $ gpio readall    // wPi column
const std::map<MotorSide, MotorPins> motor_pin_mapping = {
    {MotorSide::LEFT, {.pwm = 1, .gpio = 16, .enc_a = 4, .enc_b = 5}},
    {MotorSide::RIGHT, {.pwm = 24, .gpio = 25, .enc_a = 21, .enc_b = 22}}};
const int mode_pin = 15;
const int MAX_PWM = 1024;

class Motor {

public:
  Motor(MotorSide side);
  ~Motor();
  void run(double pwm);

private:
  MotorSide _side;
  MotorPins _pin_map;
};

#endif // MOTOR_H_
