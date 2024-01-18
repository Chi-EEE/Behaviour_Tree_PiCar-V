#include "TB6612.h"

// Made with the help of ChatGPT

TB6612::TB6612(int motor_pin, int pwm_pin) : motor_pin(motor_pin), pwm_pin(pwm_pin)
{
    gpioSetMode(this->motor_pin, PI_OUTPUT);
    gpioSetMode(this->pwm_pin, PI_OUTPUT);
}

void TB6612::setPWM(int value)
{
    gpioPWM(this->pwm_pin, value);
}

void TB6612::forward()
{
    gpioWrite(this->motor_pin, this->offset);
}

void TB6612::backward()
{
    gpioWrite(this->motor_pin, !this->offset);
}

void TB6612::stop()
{
    gpioWrite(this->motor_pin, 0);
    gpioPWM(this->pwm_pin, 0);
}

void TB6612::setOffset(bool offset)
{
    this->offset = offset;
}

const int &TB6612::getMotorPin() const
{
    return this->motor_pin;
}

const int &TB6612::getPWMPin() const
{
    return this->pwm_pin;
}
