#ifdef __linux__
#ifndef SERVO_H
#define SERVO_H

#include <algorithm>
#include <memory>

#include <PCA9685.h>

namespace car::system::movement::devices
{
	class Servo
	{
	private:
		/**
		 * Following method clamps the x to in_min and in_max.
		 * Afterwards, it puts the result of that into the range of out_min and out_max
		 */
		static int map(int x, int in_min, int in_max, int out_min, int out_max)
		{
			return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
		}

		static constexpr int MIN_PULSE_WIDTH = 900;
		static constexpr int MAX_PULSE_WIDTH = 2100;
		static constexpr int FREQUENCY = 50;

	public:
		Servo(std::shared_ptr<PCA9685> pwm, int channel);

		// Some of the code was from: https://github.com/chaoticmachinery/pca9685
		int getAnalogAngle() const;

		int getAngle() const;

		// Some of the code was from: https://github.com/chaoticmachinery/pca9685
		void setAngle(const int angle);

		void reset();

	private:
		const std::shared_ptr<PCA9685> pwm_;
		const int channel_;

		int angle_;
	};
} // namespace car::system::movement::wheels

#endif
#endif // __linux__