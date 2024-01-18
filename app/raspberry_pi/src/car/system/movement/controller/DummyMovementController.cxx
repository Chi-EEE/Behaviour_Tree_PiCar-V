#ifndef DUMMYWHEELCONTROLLER_CXX
#define DUMMYWHEELCONTROLLER_CXX

#pragma once

#include <spdlog/spdlog.h>

#include "AbstractMovementController.cxx"

namespace car::system::movement::controller
{
	class DummyMovementController : public AbstractMovementController
	{
	public:
		void initialize() {};

		void setRearWheelsSpeed(const int& speed) override {
			spdlog::info("[DUMMY] Setting Left Rear Wheel speed to {}", speed);
		}

		void setRearLeftWheelSpeed(const int& speed) override
		{
			spdlog::info("[DUMMY] Setting Left Rear Wheel speed to {}", speed);
		}

		void setRearRightWheelSpeed(const int& speed) override
		{
			spdlog::info("[DUMMY] Setting Right Rear Wheel speed to {}", speed);
		}

		void setFrontWheelsAngle(const float& angle) override
		{
			spdlog::info("[DUMMY] Setting Front Wheels angle to {}", angle);
		}

		void setCameraServo1Angle(const float& angle) override
		{
			spdlog::info("[DUMMY] Setting Camera Servo 1 angle to {}", angle);
		}

		void setCameraServo2Angle(const float& angle) override
		{
			spdlog::info("[DUMMY] Setting Camera Servo 2 angle to {}", angle);
		}

		void setRearWheelDirectionToForward() override {
			spdlog::info("[DUMMY] Both Rear Wheels are set to to move forward");
		}

		void setRearWheelDirectionToBackward() override {
			spdlog::info("[DUMMY] Both Rear Wheels are set to move backward");
		}

	private:
	};
} // namespace car::system::movement::controller

#endif