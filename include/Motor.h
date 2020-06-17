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
  int enc_a[2]; // One for rising edge and one for falling edge. Because of high
                // latency in interrupts in RPi, we cannot read gpio in an ISR
  int enc_b[2];
};

// Assuming BCM GPIO numbering scheme
// $ gpio readall    // BCM column
const std::map<MotorSide, MotorPins> motor_pin_mapping = {
    {MotorSide::LEFT,
     {.pwm = 18, .gpio = 15, .enc_a = {23, 20}, .enc_b = {24, 16}}},
    {MotorSide::RIGHT,
     {.pwm = 19, .gpio = 26, .enc_a = {8, 7}, .enc_b = {25, 12}}}};
const int mode_pin = 14;
constexpr double max_pwm = 1.0;
const uint pwm_freq = 25000;

class Motor {

public:
  Motor(MotorSide side, int host_id = -1);
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
  int _host_id;

};

#endif // MOTOR_H_
