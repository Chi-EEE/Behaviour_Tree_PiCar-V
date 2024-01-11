#ifndef MOVEMENTSYSTEM_CXX
#define MOVEMENTSYSTEM_CXX

#pragma once

#include <memory>

#include "controller/AbstractWheelController.cxx"

using namespace car::system::movement::controller;

namespace car::system::movement {
	class MovementSystem
	{
	public:
		MovementSystem(std::unique_ptr<AbstractWheelController> wheel_controller) : wheel_controller(std::move(wheel_controller))
		{
		};

		void initialize()
		{
			this->wheel_controller->initialize();
		}

		void start() {

		}

		void stop() {
		
		}

#pragma region Wheels
		void setRearWheelsSpeed(const int& speed) {
			this->wheel_controller->setRearWheelsSpeed(speed);
		}

		void setFrontWheelsAngle(const float& angle) {
			this->wheel_controller->setFrontWheelsAngle(angle);
		}

		void setRearLeftWheelSpeed(const int& speed)
		{
			this->wheel_controller->setRearLeftWheelSpeed(speed);
		}

		void setRearRightWheelSpeed(const int& speed)
		{
			this->wheel_controller->setRearRightWheelSpeed(speed);
		}

		void setFrontLeftWheelAngle(const float& angle)
		{
			this->wheel_controller->setFrontLeftWheelAngle(angle);
		}

		void setFrontRightWheelAngle(const float& angle)
		{
			this->wheel_controller->setFrontRightWheelAngle(angle);
		}

		void setRearWheelDirectionToForwards()
		{
			this->wheel_controller->setRearWheelDirectionToForwards();
		}
		void setRearWheelDirectionToBackwards()
		{
			this->wheel_controller->setRearWheelDirectionToBackwards();
		}
#pragma endregion


		~MovementSystem() {
		};

	private:
		std::unique_ptr<AbstractWheelController> wheel_controller;
	};
};

#endif