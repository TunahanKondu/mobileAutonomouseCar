#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <string>
#include "pwm.h"
#include "QThread"
#include <jetgpio.h>

PWM::PWM(int pin) : period(50), dutyCycle(500000) {
    int pwmchipNumber = getPwmChip(pin);
    pwmPin = pwmchipNumber;

    if (pwmchipNumber != -1) {
        pwmchip = "pwmchip" + QString::number(pwmchipNumber);
        qDebug() << "Using " << pwmchip << " for pin " << pin;
    } else {
        qDebug() << "Invalid pin number.";
    }
}

PWM::~PWM() {

}

int PWM::getPwmChip(int pin) {

    if (pin == 15) {
        return 0;  // pin 15 corresponds to pwmchip0
    } else if (pin == 18) {
        return 2;  // pin 33 corresponds to pwmchip2
    } else if (pin == 13) {
        return 3;  // pin 32 corresponds to pwmchip3
    }
    return -1;  // Invalid pin number
}

bool PWM::exportPWM() {
    QFile file(QString("/sys/class/pwm/%1/export").arg(pwmchip));  // Use dynamic pwmchip
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << "0";
        file.close();
        return true;
    }
    return false;
}

void PWM::setPeriod(int frequency) {
    period = 1000000000 / frequency;
    QFile file(QString("/sys/class/pwm/%1/pwm0/period").arg(pwmchip));  // Always use pwm0 (fixed channel)
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << period;
        file.close();
    }
}

void PWM::setDutyCycle(int dc) {
    dutyCycle = dc;
    QFile file(QString("/sys/class/pwm/%1/pwm0/duty_cycle").arg(pwmchip));  // Always use pwm0 (fixed channel)
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << dutyCycle;
        file.close();
    }
}

void PWM::enablePWM() {
    QFile file(QString("/sys/class/pwm/%1/pwm0/enable").arg(pwmchip));  // Always use pwm0 (fixed channel)
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << 1;
        file.close();
    }
}

void PWM::disablePWM() {
    QFile file(QString("/sys/class/pwm/%1/pwm0/enable").arg(pwmchip));  // Always use pwm0 (fixed channel)
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << 0;
        file.close();
    }
}

void PWM::setServoAngle(int angle) {
    if (angle < 0) {
        angle = 0;
        qDebug() << "Angle too low. Clamping to 0.";
    } else if (angle > 180) {
        angle = 180;
        qDebug() << "Angle too high. Clamping to 180.";
    }

    int pulseWidth = 500000 + (angle * 2000000 / 180);
    qDebug() << pulseWidth;

    setDutyCycle(pulseWidth);
}

void PWM::setStepperMotorDirection(int directionPin, const std::string& direction) {
    gpioInitialise();
    gpioSetMode(directionPin, JET_OUTPUT);

    if (direction == "right") {
        gpioWrite(directionPin, 1);
        qDebug() << "Motor direction: Right (Clockwise)";
    }
    else if (direction == "left") {
        gpioWrite(directionPin, 0);
        qDebug() << "Motor direction: Left (Counterclockwise)";
    }
    else {
        qDebug() << "Invalid direction input. Please use 'right' or 'left'.";
    }
}

void PWM::setStepperMotorSpeed(int speed) {

    int minFrequency = 100;
    int maxFrequency = 1000;

    int mappedFrequency = minFrequency + ((maxFrequency - minFrequency) * speed) / 100;

    setDutyCycle(1000000);
    QThread::msleep(100);
    setPeriod(mappedFrequency);
}
