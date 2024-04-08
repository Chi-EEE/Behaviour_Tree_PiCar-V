#ifdef __linux__
#include "car/system/movement/devices/RearWheel.h"

#include <algorithm>
#include <memory>

#include <PCA9685.h>
#include <TB6612.h>

// Made with the help of ChatGPT

namespace car::system::movement::devices
{
    RearWheel::RearWheel(std::shared_ptr<PCA9685> pwm, std::unique_ptr<TB6612> motor) : pwm_(pwm),
                                                                             motor_(std::move(motor))
    {
        this->motor_->setOffset(false);
        this->speed_ = 0;
    }

    void RearWheel::forward()
    {
        this->motor_->forward();
    }

    void RearWheel::backward()
    {
        this->motor_->backward();
    }

    void RearWheel::stop()
    {
        this->motor_->stop();
    }

    int RearWheel::getSpeed() const
    {
        return this->speed_;
    }

    void RearWheel::setSpeed(const int speed)
    {
        const int new_speed = std::clamp(speed, 0, 100);
        // This is to prevent the motor from being set to the same speed and causing a PWM signal to be sent again
        if (new_speed == this->speed_)
        {
            return;
        }
        this->speed_ = new_speed;
        const int pulse_wide = (this->speed_ / 100.0f) * 4095;
        this->pwm_->setPWM(this->motor_->getPWMPin(), 0, pulse_wide);
    }

    void RearWheel::ready()
    {
        this->stop();
    }
} // namespace car::system::movement::wheels

#endif