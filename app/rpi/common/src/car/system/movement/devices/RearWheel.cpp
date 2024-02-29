#ifdef __linux__
#include "car/system/movement/devices/RearWheel.h"

#include <algorithm>
#include <memory>

#include <PCA9685.h>
#include <TB6612.h>

// Made with the help of ChatGPT

namespace car::system::movement::devices
{
    RearWheel::RearWheel(std::shared_ptr<PCA9685> pwm, std::unique_ptr<TB6612> motor) : pwm(pwm),
                                                                             motor(std::move(motor))
    {
        this->motor->setOffset(false);
        this->speed = 0;
    }

    void RearWheel::forward()
    {
        this->motor->forward();
    }

    void RearWheel::backward()
    {
        this->motor->backward();
    }

    void RearWheel::stop()
    {
        this->motor->stop();
    }

    int RearWheel::getSpeed() const
    {
        return this->speed;
    }

    void RearWheel::setSpeed(const int &speed)
    {
        const int new_speed = std::clamp(speed, 0, 100);
        if (new_speed == this->speed)
        {
            return;
        }
        this->speed = new_speed;
        const int pulse_wide = (this->speed / 100.0f) * 4095;
        this->pwm->setPWM(this->motor->getPWMPin(), 0, pulse_wide);
    }

    void RearWheel::ready()
    {
        this->stop();
    }
} // namespace car::system::movement::wheels

#endif