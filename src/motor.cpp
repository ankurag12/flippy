#include <wiringPi.h>
#include "motor.h"
#include "utils.h"
#include "pin_mapping.h"

Motor::Motor(int id)
    :   id_(id), pin_map_(get_motor_pin_map(id)) {
    wiringPiSetupGpio();
    pinMode(pin_map_->gpio, OUTPUT);
    pinMode(pin_map_->pwm, PWM_OUTPUT);
    pinMode(pin_map_->enc_a, INPUT);
    pinMode(pin_map_->enc_b, INPUT);    
}

void Motor::run(double pwm) {
    
    // Clip pwm between -1.0 and 1.0
    pwm = (pwm < -1.0) ? -1.0 : (pwm > 1.0) ? 1.0 : pwm;
    
    digitalWrite(pin_map_->gpio, pwm<0);
    
    const int MAX_PWM = 1000;
    // Scale pwm between 0 and MAX_PWM
    int pwm_int = int(pwm * MAX_PWM);
    pwmWrite(pin_map_->pwm, pwm_int);
    
}

void Motor::brake() {
    
    
}
