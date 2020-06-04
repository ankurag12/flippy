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
  set_pwm_mode(_host_id, _pin_map.pwm);

  // Encoders
  set_digital_io_mode(_host_id, _pin_map.enc_a, DigitalIoMode::READ_INPUT);
  set_digital_io_mode(_host_id, _pin_map.enc_b, DigitalIoMode::READ_INPUT);

  // Set the inital encoder readings as "last"
  // Configure rising and falling edge ISRs on pins for encoders
  // Ideally this section should be wrapped in an "interrupts disabled" zone
  if (_side == MotorSide::LEFT) {
    g_last_enc_ab_state_left =
        (read_digital_io(_host_id, _pin_map.enc_a) << 1) |
        read_digital_io(_host_id, _pin_map.enc_b);
    set_hw_interrupt(_host_id, _pin_map.enc_a, EdgeType::EITHER,
                     &ISR_any_edge_left);
    set_hw_interrupt(_host_id, _pin_map.enc_b, EdgeType::EITHER,
                     &ISR_any_edge_left);
  } else if (_side == MotorSide::RIGHT) {
    g_last_enc_ab_state_right =
        (read_digital_io(_host_id, _pin_map.enc_a) << 1) |
        read_digital_io(_host_id, _pin_map.enc_b);
    set_hw_interrupt(_host_id, _pin_map.enc_a, EdgeType::EITHER,
                     &ISR_any_edge_right);
    set_hw_interrupt(_host_id, _pin_map.enc_b, EdgeType::EITHER,
                     &ISR_any_edge_right);
  }
}

Motor::~Motor() { run(0); }

void Motor::run(double pwm) {

  // Clip pwm between -1.0 and 1.0
  pwm = (pwm < -1.0) ? -1.0 : (pwm > 1.0) ? 1.0 : pwm;
  write_digital_io(_host_id, _pin_map.gpio, pwm < 0);

  // Scale pwm between 0 and max_pwm
  int pwm_int = abs(int(pwm * max_pwm));
  write_pwm_dutycycle(_host_id, _pin_map.pwm, pwm_int);
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
