#ifdef __linux__
#ifndef CARWHEELCONTROLLER_CXX
#define CARWHEELCONTROLLER_CXX

#pragma once

#include <memory>
#include <thread>
#include <chrono>

#include <PCA9685.h>

#include "AbstractWheelController.cxx"

#include "../wheels/FrontWheel.cxx"
#include "../wheels/RearWheel.cxx"

namespace car::system::movement::controller
{
	constexpr int MIN_PULSE_WIDTH = 900;
	constexpr int MAX_PULSE_WIDTH = 2100;
	constexpr int FREQUENCY = 50;
	class CarWheelController : public AbstractWheelController
	{
	public:
		CarWheelController() {};
		~CarWheelController() {};

		void initialize() override {
			this->pwm->init(1, 0x40);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			this->pwm->setPWMFreq(FREQUENCY);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

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

	private:
		std::unique_ptr<PCA9685> pwm;

	};
} // namespace car::system::movement::controller

#endif
#endif // __linux__