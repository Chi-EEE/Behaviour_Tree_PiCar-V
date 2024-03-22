#ifdef __linux__
#ifndef REARWHEEL_H
#define REARWHEEL_H

#include <memory>

#include <PCA9685.h>
#include <TB6612.h>

// Made with the help of ChatGPT

namespace car::system::movement::devices
{
	class RearWheel
	{
	public:
		RearWheel(std::shared_ptr<PCA9685> pwm, std::unique_ptr<TB6612> motor);

		void forward();

		void backward();

		void stop();

		int getSpeed() const;

		void setSpeed(const int speed);

		void ready();

	private:
		std::shared_ptr<PCA9685> pwm_;
		std::unique_ptr<TB6612> motor;

		int speed_;
	};
} // namespace car::system::movement::wheels

#endif
#endif