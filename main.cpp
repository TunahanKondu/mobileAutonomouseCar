#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include "pwm.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PWM step(15);
    PWM servo(32);

    step.exportPWM();
    step.setStepperMotorSpeed(99);
    step.enablePWM();
    step.setStepperMotorDirection(7, "right");
    QThread::sleep(5);
    step.setStepperMotorDirection(7, "left");

    servo.exportPWM();
    servo.setPeriod(50);
    servo.setServoAngle(90);
    servo.enablePWM();

    return a.exec();
}
