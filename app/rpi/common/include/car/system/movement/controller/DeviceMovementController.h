#ifdef __linux__
#ifndef DEVICEMOVEMENTCONTROLLER_H
#define DEVICEMOVEMENTCONTROLLER_H

#pragma once

#include <memory>

#include "AbstractMovementController.h"

#include "car/system/movement/devices/Servo.h"
#include "car/system/movement/devices/RearWheel.h"

using namespace car::system::movement::devices;

namespace car::system::movement::controller
{
	static constexpr int Motor_A = 17;
	static constexpr int Motor_B = 27;
	static constexpr int PWM_A = 4;
	static constexpr int PWM_B = 5;

	static constexpr int MIN_PULSE_WIDTH = 900;
	static constexpr int MAX_PULSE_WIDTH = 2100;
	static constexpr int FREQUENCY = 50;

	static constexpr int BUS_NUMBER = 1;

	class DeviceMovementController : public AbstractMovementController
	{
	public:
		[[nodiscard]] DeviceMovementController();

		void initialize() final override;

		void stop() final override;

		void terminate() final override;

		void setRearWheelsSpeed(const int speed) final override;

		void setRearLeftWheelSpeed(const int speed) final override;

		void setRearRightWheelSpeed(const int speed) final override;

		void setFrontWheelsAngle(const float angle) final override;

		void setCameraServo1Angle(const float angle) final override;

		void setCameraServo2Angle(const float angle) final override;

		void setRearWheelsDirectionToForward() final override;

		void setRearLeftWheelDirectionToForward() final override;

		void setRearRightWheelDirectionToForward() final override;

		void setRearWheelsDirectionToBackward() final override;

		void setRearLeftWheelDirectionToBackward() final override;

		void setRearRightWheelDirectionToBackward() final override;

	private:
		std::shared_ptr<PCA9685> pwm;

		std::unique_ptr<Servo> front_wheels_;
		std::unique_ptr<Servo> camera_servo_1_;
		std::unique_ptr<Servo> camera_servo_2_;

		std::unique_ptr<RearWheel> rear_left_wheel_;
		std::unique_ptr<RearWheel> rear_right_wheel_;
	};
} // namespace car::system::movement::controller

#endif
#endif // __linux__