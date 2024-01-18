#ifndef DUMMYWHEELCONTROLLER_CXX
#define DUMMYWHEELCONTROLLER_CXX

#pragma once

#include "AbstractMovementController.cxx"

namespace car::system::movement::controller
{
	class DummyMovementController : public AbstractMovementController
	{
	public:
		void initialize() {};

		void setRearWheelsSpeed(const int& speed) override {

		}

		void setRearLeftWheelSpeed(const int& speed) override {

		}

		void setRearRightWheelSpeed(const int& speed) override {

		}

		void setFrontWheelsAngle(const float& angle) override
		{
		
		}

		void setCameraServo1Angle(const float& angle) override
		{
		
		}

		void setCameraServo2Angle(const float& angle) override
		{

		}

		void setRearWheelDirectionToForward() override {

		}

		void setRearWheelDirectionToBackward() override {

		}
	private:
	};
} // namespace car::system::movement::controller

#endif