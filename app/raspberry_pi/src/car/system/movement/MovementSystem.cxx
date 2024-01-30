#ifndef MOVEMENTSYSTEM_CXX
#define MOVEMENTSYSTEM_CXX

#pragma once

#include <memory>

#include "controller/AbstractMovementController.cxx"

using namespace car::system::movement::controller;

namespace car::system::movement
{
	class MovementSystem
	{
	public:
		MovementSystem(std::unique_ptr<AbstractMovementController> movement_controller) : movement_controller(std::move(movement_controller)){};

		void initialize()
		{
			this->movement_controller->initialize();
		}

		void start()
		{
		}

		void stop()
		{
			this->movement_controller->stop();
		}

		void terminate()
		{
			this->movement_controller->terminate();
		}

#pragma region Wheels
		void setRearWheelsSpeed(const int &speed)
		{
			this->movement_controller->setRearWheelsSpeed(speed);
		}

		void setRearLeftWheelSpeed(const int &speed)
		{
			this->movement_controller->setRearLeftWheelSpeed(speed);
		}

		void setRearRightWheelSpeed(const int &speed)
		{
			this->movement_controller->setRearRightWheelSpeed(speed);
		}

		void setFrontWheelsAngle(const float &angle)
		{
			this->movement_controller->setFrontWheelsAngle(angle);
		}

		void setCameraServo1Angle(const float &angle)
		{
			this->movement_controller->setCameraServo1Angle(angle);
		}

		void setCameraServo2Angle(const float &angle)
		{
			this->movement_controller->setCameraServo2Angle(angle);
		}

		void setRearWheelsDirectionToForward()
		{
			this->movement_controller->setRearWheelsDirectionToForward();
		}

		void setRearLeftWheelDirectionToForward()
		{
			this->movement_controller->setRearLeftWheelDirectionToForward();
		}

		void setRearRightWheelDirectionToForward()
		{
			this->movement_controller->setRearRightWheelDirectionToForward();
		}

		void setRearWheelsDirectionToBackward()
		{
			this->movement_controller->setRearWheelsDirectionToBackward();
		}

		void setRearLeftWheelDirectionToBackward()
		{
			this->movement_controller->setRearLeftWheelDirectionToBackward();
		}

		void setRearRightWheelDirectionToBackward()
		{
			this->movement_controller->setRearRightWheelDirectionToBackward();
		}
#pragma endregion

		~MovementSystem(){};

	private:
		std::unique_ptr<AbstractMovementController> movement_controller;
	};
};

#endif