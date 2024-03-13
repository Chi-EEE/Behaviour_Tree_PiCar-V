#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#pragma once

#include <memory>

#include "car/system/movement/controller/AbstractMovementController.h"

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
		void setRearWheelsSpeed(const int &speed) const
		{
			this->movement_controller->setRearWheelsSpeed(speed);
		}

		void setRearLeftWheelSpeed(const int &speed) const
		{
			this->movement_controller->setRearLeftWheelSpeed(speed);
		}

		void setRearRightWheelSpeed(const int &speed) const
		{
			this->movement_controller->setRearRightWheelSpeed(speed);
		}

		void setFrontWheelsAngle(const float &angle) const
		{
			this->movement_controller->setFrontWheelsAngle(angle);
		}

		void setCameraServo1Angle(const float &angle) const
		{
			this->movement_controller->setCameraServo1Angle(angle);
		}

		void setCameraServo2Angle(const float &angle) const
		{
			this->movement_controller->setCameraServo2Angle(angle);
		}

		void setRearWheelsDirectionToForward() const
		{
			this->movement_controller->setRearWheelsDirectionToForward();
		}

		void setRearLeftWheelDirectionToForward() const
		{
			this->movement_controller->setRearLeftWheelDirectionToForward();
		}

		void setRearRightWheelDirectionToForward() const
		{
			this->movement_controller->setRearRightWheelDirectionToForward();
		}

		void setRearWheelsDirectionToBackward() const
		{
			this->movement_controller->setRearWheelsDirectionToBackward();
		}

		void setRearLeftWheelDirectionToBackward() const
		{
			this->movement_controller->setRearLeftWheelDirectionToBackward();
		}

		void setRearRightWheelDirectionToBackward() const
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