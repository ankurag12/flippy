#include "Motor.h"
#include "Encoder.h"
#include "utils.h"
#include <cmath>
#include <iostream>
#include <wiringPi.h>

Motor::Motor(MotorSide side) : _side(side) {
  wiringPiSetup();

  // Configure mode pin of motor driver DRV8835
  pinMode(mode_pin, OUTPUT);
  digitalWrite(mode_pin, 1);

  // Configure pins for PWM, phase and encoders
  _pin_map = motor_pin_mapping.at(_side);
  pinMode(_pin_map.gpio, OUTPUT);
  pinMode(_pin_map.pwm, PWM_OUTPUT);
  pinMode(_pin_map.enc_a, INPUT);
  pinMode(_pin_map.enc_b, INPUT);

  // Set the inital encoder readings as "last"
  // Configure rising and falling edge ISRs on pins for encoders
  // Ideally this section should be wrapped in an "interrupts disabled" zone
  if (_side == MotorSide::LEFT) 
  {
    g_last_enc_ab_state_left = (digitalRead(_pin_map.enc_a) << 1) | digitalRead(_pin_map.enc_b);
    wiringPiISR(_pin_map.enc_a, INT_EDGE_BOTH, &ISR_any_edge_left);
    wiringPiISR(_pin_map.enc_b, INT_EDGE_BOTH, &ISR_any_edge_left);
  } else if (_side == MotorSide::RIGHT) {
    g_last_enc_ab_state_right = (digitalRead(_pin_map.enc_a) << 1) | digitalRead(_pin_map.enc_b);
    wiringPiISR(_pin_map.enc_a, INT_EDGE_BOTH, &ISR_any_edge_right);
    wiringPiISR(_pin_map.enc_b, INT_EDGE_BOTH, &ISR_any_edge_right);
  }
}

Motor::~Motor() { run(0); }

void Motor::run(double pwm) {

  // Clip pwm between -1.0 and 1.0
  pwm = (pwm < -1.0) ? -1.0 : (pwm > 1.0) ? 1.0 : pwm;
  digitalWrite(_pin_map.gpio, pwm < 0);

  // Scale pwm between 0 and max_pwm
  int pwm_int = abs(int(pwm * max_pwm));
  pwmWrite(_pin_map.pwm, pwm_int);
}

int Motor::get_current_tick_count() {
  if (_side == MotorSide::LEFT) {
    return g_tick_count_left;
  } else if (_side == MotorSide::RIGHT) {
    return g_tick_count_right;
  }
  std::cerr << "Something wrong!" << std::endl;
  return 0;
}

double Motor::get_rev_per_sec() { return 0.0; }
