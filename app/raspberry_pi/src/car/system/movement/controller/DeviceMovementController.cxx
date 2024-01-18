#ifdef __linux__
#ifndef DEVICEMOVEMENTCONTROLLER_CXX
#define DEVICEMOVEMENTCONTROLLER_CXX

#pragma once

#include <memory>
#include <thread>
#include <chrono>

#include <PCA9685.h>

#include "AbstractMovementController.cxx"

#include "../devices/Servo.cxx"
#include "../devices/RearWheel.cxx"

using namespace car::system::movement::devices;

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

	class DeviceMovementController : public AbstractMovementController
	{
	public:
		DeviceMovementController()
		{
			gpioInitialise();
			this->pwm = std::make_shared<PCA9685>();
			this->rear_left_wheel = std::make_unique<RearWheel>(
				this->pwm,
				std::make_unique<TB6612>(Motor_B, PWM_B));
			this->rear_right_wheel = std::make_unique<RearWheel>(
				this->pwm,
				std::make_unique<TB6612>(Motor_A, PWM_A));
			this->front_wheels = std::make_unique<Servo>(
				this->pwm,
				0);
			this->camera_servo_1 = std::make_unique<Servo>(
				this->pwm,
				1);
			this->camera_servo_2 = std::make_unique<Servo>(
				this->pwm,
				2);
		};

		~DeviceMovementController()
		{
			this->front_wheels->reset();
			this->camera_servo_1->reset();
			this->camera_servo_2->reset();
			this->rear_left_wheel->stop();
			this->rear_right_wheel->stop();
			this->pwm->reset();
			gpioTerminate();
		};

		void initialize() override
		{
			this->pwm->init(BUS_NUMBER, 0x40);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			this->pwm->setPWMFreq(FREQUENCY);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		void setRearWheelsSpeed(const int &speed) override
		{
			spdlog::info("Both Rear Wheels speed are set to {}", speed);
			this->rear_left_wheel->setSpeed(speed);
			this->rear_right_wheel->setSpeed(speed);
		}

		void setRearLeftWheelSpeed(const int &speed) override
		{
			spdlog::info("Left Rear Wheel speed is set to {}", speed);
			this->rear_left_wheel->setSpeed(speed);
		}

		void setRearRightWheelSpeed(const int &speed) override
		{
			spdlog::info("Right Rear Wheel speed is set to {}", speed);
			this->rear_right_wheel->setSpeed(speed);
		}

		void setFrontWheelsAngle(const float &angle) override
		{
			spdlog::info("Front Wheels angle is set to {}", angle);
			this->front_wheels->setAngle(angle);
		}

		void setCameraServo1Angle(const float &angle) override
		{
			spdlog::info("Camera Servo 1 angle is set to {}", angle);
			this->camera_servo_1->setAngle(angle);
		}

		void setCameraServo2Angle(const float &angle) override
		{
			spdlog::info("Camera Servo 2 angle is set to {}", angle);
			this->camera_servo_2->setAngle(angle);
		}

		void setRearWheelsDirectionToForward() override
		{
			spdlog::info("Both Rear Wheels are set to move forward");
			this->rear_left_wheel->forward();
			this->rear_right_wheel->forward();
		}

		void setRearLeftWheelDirectionToForward() override
		{
			spdlog::info("Left Rear Wheel is set to move forward");
			this->rear_left_wheel->forward();
		}

		void setRearRightWheelDirectionToForward() override
		{
			spdlog::info("Right Rear Wheel is set to move forward");
			this->rear_right_wheel->forward();
		}

		void setRearWheelsDirectionToBackward() override
		{
			spdlog::info("Both Rear Wheels are set to move backward");
			this->rear_left_wheel->backward();
			this->rear_right_wheel->backward();
		}

		void setRearLeftWheelDirectionToBackward() override
		{
			spdlog::info("Left Rear Wheel is set to move backward");
			this->rear_left_wheel->backward();
		}

		void setRearRightWheelDirectionToBackward() override
		{
			spdlog::info("Right Rear Wheel is set to move backward");
			this->rear_right_wheel->backward();
		}

	private:
		std::shared_ptr<PCA9685> pwm;

		std::unique_ptr<Servo> front_wheels;
		std::unique_ptr<Servo> camera_servo_1;
		std::unique_ptr<Servo> camera_servo_2;

		std::unique_ptr<RearWheel> rear_left_wheel;
		std::unique_ptr<RearWheel> rear_right_wheel;
	};
} // namespace car::system::movement::controller

#endif
#endif // __linux__