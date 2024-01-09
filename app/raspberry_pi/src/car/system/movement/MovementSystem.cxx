#ifndef MOVEMENTSYSTEM_CXX
#define MOVEMENTSYSTEM_CXX

#pragma once

#include <memory>

#include "../messaging/commands/SpeedCommand.cxx"
#include "../messaging/commands/AngleCommand.cxx"

#include "controller/AbstractWheelController.cxx"

using namespace car::system::movement::controller;
using namespace car::system::messaging::commands;

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
		void setRearWheelsSpeed(const SpeedCommand& speed_command) {
			this->wheel_controller->setRearWheelsSpeed(speed_command);
		}

		void setFrontWheelsAngle(const AngleCommand& angle_command) {
			this->wheel_controller->setFrontWheelsAngle(angle_command);
		}

		void setRearLeftWheelSpeed(const SpeedCommand& speed_command)
		{
			this->wheel_controller->setRearLeftWheelSpeed(speed_command);
		}

		void setRearRightWheelSpeed(const SpeedCommand& speed_command)
		{
			this->wheel_controller->setRearRightWheelSpeed(speed_command);
		}

		void setFrontLeftWheelAngle(const AngleCommand& angle_command)
		{
			this->wheel_controller->setFrontLeftWheelAngle(angle_command);
		}

		void setFrontRightWheelAngle(const AngleCommand& angle_command)
		{
			this->wheel_controller->setFrontRightWheelAngle(angle_command);
		}
#pragma endregion


		~MovementSystem() {
		};

	private:
		std::unique_ptr<AbstractWheelController> wheel_controller;
	};
};

#endif