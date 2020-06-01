#ifndef MOTOR_H_
#define MOTOR_H_

#include <map>

extern volatile int g_tick_count_left;
extern volatile int g_tick_count_right;

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
    {MotorSide::RIGHT, {.pwm = 24, .gpio = 25, .enc_a = 6, .enc_b = 10}}};
const int mode_pin = 15;
const uint MAX_PWM = 1024;

class Motor {

public:
  Motor(MotorSide side);
  ~Motor();
  void run(double pwm);
  int get_current_tick_count();
  double get_rev_per_sec();

private:
  MotorSide _side;
  MotorPins _pin_map;
  const double _no_load_rpm = 430;
  const double _gear_ratio = 75.81;
  const uint _enc_counts_per_rev = 12;
  int _current_tick_count = 0;
  int _prev_tick_count = 0;

};

#endif // MOTOR_H_
