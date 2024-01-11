#ifndef DUMMYWHEELCONTROLLER_CXX
#define DUMMYWHEELCONTROLLER_CXX

#pragma once

#include "AbstractWheelController.cxx"

namespace car::system::movement::controller
{
	class DummyWheelController : public AbstractWheelController
	{
	public:
		void initialize() {};

		void setRearWheelsSpeed(const SpeedCommand& speed_command) override {

		}

		void setFrontWheelsAngle(const AngleCommand& angle_command) override {

		}

		void setRearLeftWheelSpeed(const SpeedCommand& speed_command) override {

		}

		void setRearRightWheelSpeed(const SpeedCommand& speed_command) override {

		}

		void setFrontLeftWheelAngle(const AngleCommand& angle_command) override {

		}

		void setFrontRightWheelAngle(const AngleCommand& angle_command) override {

		}

		void setRearWheelDirectionToForwards() override {

		}

		void setRearWheelDirectionToBackwards() override {

		}
	private:
	};
} // namespace car::system::movement::controller

#endif