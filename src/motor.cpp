#include "motor.h"
#include "utils.h"
#include <wiringPi.h>
#include <iostream>
#include <cmath>

Motor::Motor(MotorSide side) : _side(side) {
  wiringPiSetup();
  pinMode(mode_pin, OUTPUT);
  digitalWrite(mode_pin, 1);

  _pin_map = motor_pin_mapping.at(_side);
  pinMode(_pin_map.gpio, OUTPUT);
  pinMode(_pin_map.pwm, PWM_OUTPUT);
  pinMode(_pin_map.enc_a, INPUT);
  pinMode(_pin_map.enc_b, INPUT);
}

Motor::~Motor() {
  run(0);
    }
    
void Motor::run(double pwm) {

  // Clip pwm between -1.0 and 1.0
  pwm = (pwm < -1.0) ? -1.0 : (pwm > 1.0) ? 1.0 : pwm;
  std::cout<<"pwm = "<<pwm<<" pwm < 0 = " << (pwm < 0) << std::endl;
  digitalWrite(_pin_map.gpio, pwm < 0);

  // Scale pwm between 0 and MAX_PWM
  int pwm_int = abs(int(pwm * MAX_PWM));
  std::cout<<"pwm_int = " << pwm_int << std::endl;
  pwmWrite(_pin_map.pwm, pwm_int);
}

