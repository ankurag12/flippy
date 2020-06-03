#ifndef HWIMP_H_
#define HWIMPL_H_

typedef unsigned int uint;

enum class EdgeType {
    RISING = 0,
    FALLING = 1,
    EITHER = 2
};

enum class DigitalIoMode {
    READ_INPUT = 0,
    WRITE_OUTPUT = 1
};

// General
int init_gpio();
int end_gpio(int host_id);

// I2C
int init_i2c(int host_id, uint dev_address);
int read_i2c_byte(int host_id, int i2c_handler, uint dev_address);
int write_i2c_byte(int host_id, int i2c_handler, uint dev_address, uint data);

// Digital IO
int set_digital_io_mode(int host_id, uint pin, DigitalIoMode mode);
int write_digital_io(int host_id, uint pin, bool val);
int read_digital_io(int host_id, uint pin);

// PWM
int set_pwm_mode(int host_id, uint pin, uint range=1024, uint freq=10000);
int write_pwm_dutycycle(int host_id, uint pin, uint dutycycle);

// Interrupts
int set_hw_interrupt(int host_id, uint pin, EdgeType edge_type, void(*callback)());


#endif