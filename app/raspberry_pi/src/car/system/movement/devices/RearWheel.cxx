#ifdef __linux__
#ifndef REARWHEEL_CXX
#define REARWHEEL_CXX

#include <memory>

#include <spdlog/spdlog.h>

#include <PCA9685.h>
#include <TB6612.h>

// Made with the help of ChatGPT

namespace car::system::movement::devices
{
	class RearWheel
	{
	public:
		RearWheel(std::shared_ptr<PCA9685> pwm, std::unique_ptr<TB6612> motor) : pwm(pwm),
																				 motor(std::move(motor))
		{
			this->speed = 0;
		}

		void forward()
		{
			this->motor->forward();
		}

		void backward()
		{
			this->motor->backward();
		}

		void stop()
		{
			this->motor->stop();
		}

		int getSpeed() const
		{
			return this->speed;
		}

		void setSpeed(const int &speed)
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

		void ready()
		{
			this->stop();
		}

	private:
		std::shared_ptr<PCA9685> pwm;
		std::unique_ptr<TB6612> motor;

		int speed;
	};
} // namespace car::system::movement::wheels

#endif
#endif