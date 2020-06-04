#include "HwImpl.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>

int init_gpio() {
  wiringPiSetupGpio();
  return 0;
}

int end_gpio(int host_id) { return 0; }

int init_i2c(int host_id, uint dev_address) {
  return wiringPiI2CSetup(dev_address);
}

int read_i2c_byte(int host_id, int i2c_handler, uint dev_address) {
  return wiringPiI2CReadReg8(i2c_handler, dev_address);
}

int write_i2c_byte(int host_id, int i2c_handler, uint dev_address, uint data) {
  return wiringPiI2CWriteReg8(i2c_handler, dev_address, data);
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

int set_pwm_mode(int host_id, uint pin, uint range, uint freq) {
  pinMode(pin, PWM_OUTPUT);
  return 0;
}

int write_pwm_dutycycle(int host_id, uint pin, uint dutycycle) {
  pwmWrite(pin, dutycycle);
}

int set_hw_interrupt(int host_id, uint pin, EdgeType edge_type,
                     void (*callback)()) {
  switch (edge_type) {
  case EdgeType::RISING:
    wiringPiISR(pin, INT_EDGE_RISING, callback);
    break;
  case EdgeType::FALLING:
    wiringPiISR(pin, INT_EDGE_FALLING, callback);
    break;
  case EdgeType::EITHER:
    wiringPiISR(pin, INT_EDGE_BOTH, callback);
    break;
  default:
    std::cout << "Unknown EdgeType!" << std::endl;
    break;
  }
}
