#include "HwImpl.h"
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>

static uint pwm_range = 1024;

int init_gpio() {
  wiringPiSetupGpio();
  return 0;
}

void end_gpio(int host_id) {}

int init_i2c(int host_id, uint dev_address) {
  return wiringPiI2CSetup(dev_address);
}

int read_i2c_byte(int host_id, int i2c_handler, uint reg_address) {
  return wiringPiI2CReadReg8(i2c_handler, reg_address);
}

int write_i2c_byte(int host_id, int i2c_handler, uint reg_address, uint data) {
  return wiringPiI2CWriteReg8(i2c_handler, reg_address, data);
}

int set_digital_io_mode(int host_id, uint pin, DigitalIoMode mode) {
  if (mode == DigitalIoMode::READ_INPUT) {
    pinMode(pin, INPUT);
  } else if (mode == DigitalIoMode::WRITE_OUTPUT) {
    pinMode(pin, OUTPUT);
  }
  return 0;
}

int write_digital_io(int host_id, uint pin, bool val) {
  digitalWrite(pin, (int)val);
  return 0;
}

int read_digital_io(int host_id, uint pin) { return digitalRead(pin); }

int set_pwm_mode(int host_id, uint pin, uint freq) {
  pinMode(pin, PWM_OUTPUT);
  // WiringPi's PWM base frequency is 19.2 MHz
  // PWM_clock_freq = PWM_base / divisor
  // PWM_freq = PWM_base_freq / (divisor * range)
  // pwmSetClock expects a divisor
  int divisor = 19200000 / (freq * pwm_range);
  divisor = (divisor < 2) ? 2 : (divisor > 4096) ? 4096 : divisor;
  pwmSetClock(divisor);
  pwm_range = 19200000 / (divisor * freq);
  pwmSetRange(pwm_range);
  pwmSetMode(PWM_MODE_MS);
  return 0;
}

int write_pwm_dutycycle(int host_id, uint pin, double dutycycle) {
  return pwmWrite(pin, (uint)(dutycycle * pwm_range));
}

int set_hw_interrupt(int host_id, uint pin, EdgeType edge_type,
                     void (*isr_callback)()) {
  switch (edge_type) {
  case EdgeType::RISING:
    return wiringPiISR(pin, INT_EDGE_RISING, isr_callback);
  case EdgeType::FALLING:
    return wiringPiISR(pin, INT_EDGE_FALLING, isr_callback);
  case EdgeType::EITHER:
    return wiringPiISR(pin, INT_EDGE_BOTH, isr_callback);
  default:
    std::cout << "Unknown EdgeType!" << std::endl;
    return -1;
  }
}
