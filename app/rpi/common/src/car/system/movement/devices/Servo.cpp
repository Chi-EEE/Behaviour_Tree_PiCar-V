#ifdef __linux__
#include "car/system/movement/devices/Servo.h"

#include <algorithm>
#include <memory>

#include <PCA9685.h>
#include <spdlog/spdlog.h>

namespace car::system::movement::devices
{
    Servo::Servo(std::shared_ptr<PCA9685> pwm, int channel) : pwm(pwm), channel(channel)
    {
    }

    // Some of the code was from: https://github.com/chaoticmachinery/pca9685
    int Servo::getAnalogAngle() const
    {
        float pulse_wide = map(this->angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
        int analog_angle = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
        return analog_angle;
    }

    int Servo::getAngle() const
    {
        return this->angle;
    }

    // Some of the code was from: https://github.com/chaoticmachinery/pca9685
    void Servo::setAngle(const int &angle)
    {
        const int new_angle = std::clamp(angle, 0, 180);
        if (new_angle == this->angle)
        {
            return;
        }
        this->angle = new_angle;
        int analog_angle = getAnalogAngle();
        this->pwm->setPWM(channel, 0, analog_angle);
    }

    void Servo::reset()
    {
        this->setAngle(90);
    }
} // namespace car::system::movement::wheels

#endif // __linux__