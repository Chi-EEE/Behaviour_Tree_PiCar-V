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
		void initialize() final override{};

		void stop() final override
		{
			spdlog::info("[DUMMY] Car is stopped");
		}

		void terminate() final override{};

		void setRearWheelsSpeed(const int &speed) final override
		{
			spdlog::info("[DUMMY] Both Rear Wheels speed are set to {}", speed);
		}

		void setRearLeftWheelSpeed(const int &speed) final override
		{
			spdlog::info("[DUMMY] Left Rear Wheel speed is set to {}", speed);
		}

		void setRearRightWheelSpeed(const int &speed) final override
		{
			spdlog::info("[DUMMY] Right Rear Wheel speed is set to {}", speed);
		}

		void setFrontWheelsAngle(const float &angle) final override
		{
			spdlog::info("[DUMMY] Front Wheels angle is set to {}", angle);
		}

		void setCameraServo1Angle(const float &angle) final override
		{
			spdlog::info("[DUMMY] Camera Servo 1 angle is set to {}", angle);
		}

		void setCameraServo2Angle(const float &angle) final override
		{
			spdlog::info("[DUMMY] Camera Servo 2 angle is set to {}", angle);
		}

		void setRearWheelsDirectionToForward() final override
		{
			spdlog::info("[DUMMY] Both Rear Wheels are set to move forward");
		}

		void setRearLeftWheelDirectionToForward() final override
		{
			spdlog::info("[DUMMY] Left Rear Wheel is set to move forward");
		}

		void setRearRightWheelDirectionToForward() final override
		{
			spdlog::info("[DUMMY] Right Rear Wheel is set to move forward");
		}

		void setRearWheelsDirectionToBackward() final override
		{
			spdlog::info("[DUMMY] Both Rear Wheels are set to move backward");
		}

		void setRearLeftWheelDirectionToBackward() final override
		{
			spdlog::info("[DUMMY] Left Rear Wheel is set to move backward");
		}

		void setRearRightWheelDirectionToBackward() final override
		{
			spdlog::info("[DUMMY] Right Rear Wheel is set to move backward");
		}

	private:
	};
} // namespace car::system::movement::controller

#endif