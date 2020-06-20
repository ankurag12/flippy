#include "HwImpl.h"
#include <iostream>
#include <pigpiod_if2.h>

static uint pwm_freq;

int init_gpio() { return pigpio_start(NULL, NULL); }

void end_gpio(int host_id) { pigpio_stop(host_id); }

int init_i2c(int host_id, uint dev_address) {
  return i2c_open(host_id, 1, dev_address, 0);
}

int read_i2c_byte(int host_id, int i2c_handler, uint reg_address) {
  return i2c_read_byte_data(host_id, i2c_handler, reg_address);
}

int write_i2c_byte(int host_id, int i2c_handler, uint reg_address, uint data) {
  return i2c_write_byte_data(host_id, i2c_handler, reg_address, data);
}

int set_digital_io_mode(int host_id, uint pin, DigitalIoMode mode) {
  if (mode == DigitalIoMode::READ_INPUT) {
    return set_mode(host_id, pin, PI_INPUT);
  } else if (mode == DigitalIoMode::WRITE_OUTPUT) {
    return set_mode(host_id, pin, PI_OUTPUT);
  }
  return 0;
}

int write_digital_io(int host_id, uint pin, bool val) {
  return gpio_write(host_id, pin, (uint)val);
}

int read_digital_io(int host_id, uint pin) { return gpio_read(host_id, pin); }

int set_pwm_mode(int host_id, uint pin, uint freq) {
  // Using pigpio's hardware PWM, as the normal PWM doesn't provide good range
  // and frequency combination for good DC motor control
  // The actual PWM range is 250M/PWMfreq. However, it expects the pwm input to
  // be in the range 0 - 1,000,000 and does scaling automatically
  pwm_freq = freq;
  return hardware_PWM(host_id, pin, pwm_freq, 0);
}

int write_pwm_dutycycle(int host_id, uint pin, double dutycycle) {
  return hardware_PWM(host_id, pin, pwm_freq, (uint)(dutycycle * 1000000));
}

// LOL! What a hack!
// Even though pigpio library allows to have user data in the callback function,
// in the spirit of keeping the signature of `set_hw_interrupt` more
// interrupt-like, where ISRs can't have user data, let's just live with this
// hack!
void (*isr_callback_copy[8])();

void pigpio_callback0(int pi, unsigned user_gpio, unsigned level,
                      uint32_t tick) {
  isr_callback_copy[0]();
}
void pigpio_callback1(int pi, unsigned user_gpio, unsigned level,
                      uint32_t tick) {
  isr_callback_copy[1]();
}
void pigpio_callback2(int pi, unsigned user_gpio, unsigned level,
                      uint32_t tick) {
  isr_callback_copy[2]();
}
void pigpio_callback3(int pi, unsigned user_gpio, unsigned level,
                      uint32_t tick) {
  isr_callback_copy[3]();
}
void pigpio_callback4(int pi, unsigned user_gpio, unsigned level,
                      uint32_t tick) {
  isr_callback_copy[4]();
}
void pigpio_callback5(int pi, unsigned user_gpio, unsigned level,
                      uint32_t tick) {
  isr_callback_copy[5]();
}
void pigpio_callback6(int pi, unsigned user_gpio, unsigned level,
                      uint32_t tick) {
  isr_callback_copy[6]();
}
void pigpio_callback7(int pi, unsigned user_gpio, unsigned level,
                      uint32_t tick) {
  isr_callback_copy[7]();
}

void (*pigpio_callback[8])(int pi, unsigned user_gpio, unsigned level,
                           uint32_t tick) = {
    pigpio_callback0, pigpio_callback1, pigpio_callback2, pigpio_callback3,
    pigpio_callback4, pigpio_callback5, pigpio_callback6, pigpio_callback7};

int set_hw_interrupt(int host_id, uint pin, EdgeType edge_type,
                     void (*isr_callback)()) {
  static int i = 0;
  isr_callback_copy[i] = isr_callback;

  switch (edge_type) {
  case EdgeType::RISING:
    return callback(host_id, pin, RISING_EDGE, pigpio_callback[i++]);
  case EdgeType::FALLING:
    return callback(host_id, pin, FALLING_EDGE, pigpio_callback[i++]);
  case EdgeType::EITHER:
    return callback(host_id, pin, EITHER_EDGE, pigpio_callback[i++]);
  default:
    std::cout << "Unknown EdgeType!" << std::endl;
    return -1;
  }
}
