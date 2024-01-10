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

using namespace car::system::movement::wheels;

namespace car::system::movement::controller
{
	static constexpr int Motor_A = 17;
	static constexpr int Motor_B = 27;
	static constexpr int PWM_A = 4;
	static constexpr int PWM_B = 5;

	static constexpr int MIN_PULSE_WIDTH = 900;
	static constexpr int MAX_PULSE_WIDTH = 2100;
	static constexpr int FREQUENCY = 50;

	static constexpr int BUS_NUMBER = 1;

	class CarWheelController : public AbstractWheelController
	{
	public:
		CarWheelController() {
			this->pwm = std::make_shared<PCA9685>();
			this->rear_left_wheel = std::make_unique<RearWheel>(
				this->pwm,
				std::make_unique<TB6612>(Motor_A, PWM_A)
			);
			this->rear_right_wheel = std::make_unique<RearWheel>(
				this->pwm,
				std::make_unique<TB6612>(Motor_B, PWM_B)
			);
			this->front_left_wheel = std::make_unique<FrontWheel>(
				this->pwm,
				0
			);
			this->front_right_wheel = std::make_unique<FrontWheel>(
				this->pwm,
				1
			);
		};

		~CarWheelController() {};

		void initialize() override {
			this->pwm->init(BUS_NUMBER, 0x40);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			this->pwm->setPWMFreq(FREQUENCY);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		void setRearWheelsSpeed(const SpeedCommand& speed_command) override {
			this->setRearLeftWheelSpeed(speed_command);
			this->setRearRightWheelSpeed(speed_command);
		}

		void setFrontWheelsAngle(const AngleCommand& angle_command) override {
			this->setFrontLeftWheelAngle(angle_command);
			this->setFrontRightWheelAngle(angle_command);
		}

		void setRearLeftWheelSpeed(const SpeedCommand& speed_command) override {
			this->rear_left_wheel->setSpeed(speed_command.speed);
		}

		void setRearRightWheelSpeed(const SpeedCommand& speed_command) override {
			this->rear_right_wheel->setSpeed(speed_command.speed);
		}

		void setFrontLeftWheelAngle(const AngleCommand& angle_command) override {
			this->front_left_wheel->setAngle(angle_command.angle);
		}

		void setFrontRightWheelAngle(const AngleCommand& angle_command) override {
			this->front_right_wheel->setAngle(angle_command.angle);
		}

	private:
		std::shared_ptr<PCA9685> pwm;

		std::unique_ptr<FrontWheel> front_left_wheel;
		std::unique_ptr<FrontWheel> front_right_wheel;

		std::unique_ptr<RearWheel> rear_left_wheel;
		std::unique_ptr<RearWheel> rear_right_wheel;
	};
} // namespace car::system::movement::controller

#endif
#endif // __linux__