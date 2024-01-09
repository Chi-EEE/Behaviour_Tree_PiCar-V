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
		void set_rear_wheels_speed(const SpeedCommand& speed_command) {
			this->wheel_controller->set_rear_wheels_speed(speed_command);
		}

		void set_front_wheels_angle(const AngleCommand& angle_command) {
			this->wheel_controller->set_front_wheels_angle(angle_command);
		}

		void set_rear_left_wheel_speed(const SpeedCommand& speed_command)
		{
			this->wheel_controller->set_rear_left_wheel_speed(speed_command);
		}

		void set_rear_right_wheel_speed(const SpeedCommand& speed_command)
		{
			this->wheel_controller->set_rear_right_wheel_speed(speed_command);
		}

		void set_front_left_wheel_angle(const AngleCommand& angle_command)
		{
			this->wheel_controller->set_front_left_wheel_angle(angle_command);
		}

		void set_front_right_wheel_angle(const AngleCommand& angle_command)
		{
			this->wheel_controller->set_front_right_wheel_angle(angle_command);
		}
#pragma endregion


		~MovementSystem() {
		};

	private:
		std::unique_ptr<AbstractWheelController> wheel_controller;
	};
};

#endif