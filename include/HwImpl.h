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
void end_gpio(int host_id);

// I2C
int init_i2c(int host_id, uint dev_address);
int read_i2c_byte(int host_id, int i2c_handler, uint reg_address);
int write_i2c_byte(int host_id, int i2c_handler, uint reg_address, uint data);

// Digital IO
int set_digital_io_mode(int host_id, uint pin, DigitalIoMode mode);
int write_digital_io(int host_id, uint pin, bool val);
int read_digital_io(int host_id, uint pin);

// PWM
int set_pwm_mode(int host_id, uint pin, uint freq=8000);

int write_pwm_dutycycle(int host_id, uint pin, double dutycycle);

// Interrupts
int set_hw_interrupt(int host_id, uint pin, EdgeType edge_type, void(*isr_callback)());


#endif  // HWIMPL_H_