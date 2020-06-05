#include "Motor.h"
#include "Encoder.h"
#include "HwImpl.h"
#include "utils.h"
#include <cmath>
#include <iostream>

Motor::Motor(MotorSide side, int host_id) : _side(side), _host_id(host_id) {
  if (_host_id == -1) {
    _host_id = init_gpio();
  }

  // Configure mode pin of motor driver DRV8835
  set_digital_io_mode(_host_id, mode_pin, DigitalIoMode::WRITE_OUTPUT);
  write_digital_io(_host_id, mode_pin, 1);

  // Configure pins for PWM, phase and encoders
  _pin_map = motor_pin_mapping.at(_side);

  // Phase (direction)
  set_digital_io_mode(_host_id, _pin_map.gpio, DigitalIoMode::WRITE_OUTPUT);

  // PWM
  set_pwm_mode(_host_id, _pin_map.pwm, pwm_freq);
  
  // Encoders
  set_digital_io_mode(_host_id, _pin_map.enc_a[0], DigitalIoMode::READ_INPUT);
  set_digital_io_mode(_host_id, _pin_map.enc_a[1], DigitalIoMode::READ_INPUT);
  set_digital_io_mode(_host_id, _pin_map.enc_b[0], DigitalIoMode::READ_INPUT);
  set_digital_io_mode(_host_id, _pin_map.enc_b[1], DigitalIoMode::READ_INPUT);

  // Configure rising and falling edge ISRs on pins for encoders
  // Very dirty way! Ideally there should be a class and then statically linked
  // Because this code could be run on an RPi running a relatively hefty OS,
  // interrupts would have latency which would mean reading signal level at the
  // encoder pins in an ISR wouln't make much sense. To overcome that problem
  // and still being able to calculate motor direction, we are using a separate
  // ISR for each encoder pin, and for each edge type (4 pins/ISRs per motor)
  if (_side == MotorSide::LEFT) {
    set_hw_interrupt(_host_id, _pin_map.enc_a[0], EdgeType::RISING,
                     &ISR_rising_edge_a_left);
    set_hw_interrupt(_host_id, _pin_map.enc_a[1], EdgeType::FALLING,
                     &ISR_falling_edge_a_left);
    set_hw_interrupt(_host_id, _pin_map.enc_b[0], EdgeType::RISING,
                     &ISR_rising_edge_b_left);
    set_hw_interrupt(_host_id, _pin_map.enc_b[1], EdgeType::FALLING,
                     &ISR_falling_edge_b_left);
  } else if (_side == MotorSide::RIGHT) {
    set_hw_interrupt(_host_id, _pin_map.enc_a[0], EdgeType::RISING,
                     &ISR_rising_edge_a_right);
    set_hw_interrupt(_host_id, _pin_map.enc_a[1], EdgeType::FALLING,
                     &ISR_falling_edge_a_right);
    set_hw_interrupt(_host_id, _pin_map.enc_b[0], EdgeType::RISING,
                     &ISR_rising_edge_b_right);
    set_hw_interrupt(_host_id, _pin_map.enc_b[1], EdgeType::FALLING,
                     &ISR_falling_edge_b_right);
  }
}

Motor::~Motor() { run(0); }

void Motor::run(double pwm) {

  // Clip pwm between -max_pwm and max_pwm
  pwm = (pwm < -max_pwm) ? -max_pwm : (pwm > max_pwm) ? max_pwm : pwm;
  write_digital_io(_host_id, _pin_map.gpio, pwm < 0);
  write_pwm_dutycycle(_host_id, _pin_map.pwm, fabs(pwm));
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
