#ifndef PWM_H
#define PWM_H
#include "QString"
#include <jetgpio.h>

class PWM {
public:
    PWM(int pin);
    ~PWM();

    bool exportPWM();
    void setPeriod(int period);
    void setDutyCycle(int dutyCycle);
    void enablePWM();
    void disablePWM();
    void setServoAngle(int angle);

    void setStepperMotorDirection(int directionPin, const std::string& direction);
    void setStepperMotorSpeed(int speed);
    int getPwmChip(int pin);

public:
    int pwmPin;
    int period;
    int dutyCycle;
    int pin;
    QString pwmchip;
    int directionPin;
};

#endif // PWM_H
