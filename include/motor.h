#include "pin_config.h"

class Motor {
    
    public:
    Motor(int id);
    void run(double pwm);
    void brake();
    
    private:
    int id_;
    MotorPins* pin_map_;
    
};
