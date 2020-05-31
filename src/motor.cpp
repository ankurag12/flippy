#include "motor.h"
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

  // Configure pins for PWM and phase
  _pin_map = motor_pin_mapping.at(_side);
  pinMode(_pin_map.gpio, OUTPUT);
  pinMode(_pin_map.pwm, PWM_OUTPUT);

  // Configure rising and falling edge ISRs on pins for encoders
  if (_side == MotorSide::LEFT) {
    wiringPiISR(_pin_map.enc_a, INT_EDGE_RISING, &ISR_rising_edge_a_left);
    wiringPiISR(_pin_map.enc_a, INT_EDGE_FALLING, &ISR_falling_edge_a_left);
    wiringPiISR(_pin_map.enc_b, INT_EDGE_RISING, &ISR_rising_edge_b_left);
    wiringPiISR(_pin_map.enc_b, INT_EDGE_FALLING, &ISR_falling_edge_b_left);
  } else if (_side == MotorSide::RIGHT) {
    wiringPiISR(_pin_map.enc_a, INT_EDGE_RISING, &ISR_rising_edge_a_right);
    wiringPiISR(_pin_map.enc_a, INT_EDGE_FALLING, &ISR_falling_edge_a_right);
    wiringPiISR(_pin_map.enc_b, INT_EDGE_RISING, &ISR_rising_edge_b_right);
    wiringPiISR(_pin_map.enc_b, INT_EDGE_FALLING, &ISR_falling_edge_b_righth);
  }
}

Motor::~Motor() { run(0); }

void Motor::run(double pwm) {

  // Clip pwm between -1.0 and 1.0
  pwm = (pwm < -1.0) ? -1.0 : (pwm > 1.0) ? 1.0 : pwm;
  digitalWrite(_pin_map.gpio, pwm < 0);

  // Scale pwm between 0 and MAX_PWM
  int pwm_int = abs(int(pwm * MAX_PWM));
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
