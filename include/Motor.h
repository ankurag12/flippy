#ifndef MOTOR_H_
#define MOTOR_H_

#include <map>

// Globals from elsewhere
extern volatile int g_tick_count_left;
extern volatile int g_tick_count_right;
extern volatile int g_last_enc_ab_state_left;
extern volatile int g_last_enc_ab_state_right;

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
const uint max_pwm = 500;

class Motor {

public:
  Motor(MotorSide side);
  ~Motor();
  void run(double pwm);
  int get_current_tick_count();
  double get_rev_per_sec();
  static constexpr double no_load_rpm = 430;
  static constexpr double gear_ratio = 75.81;
  static constexpr uint enc_counts_per_rev = 12;

private:
  MotorSide _side;
  MotorPins _pin_map;
  int _current_tick_count = 0;
  int _prev_tick_count = 0;
};

#endif // MOTOR_H_
