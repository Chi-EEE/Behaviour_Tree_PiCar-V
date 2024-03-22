#ifdef __linux__
#include "car/system/movement/controller/DeviceMovementController.h"

#include <memory>
#include <thread>
#include <chrono>

#include <PCA9685.h>

#include <spdlog/spdlog.h>

#include "car/system/movement/controller/AbstractMovementController.h"

#include "car/system/movement/devices/Servo.h"
#include "car/system/movement/devices/RearWheel.h"

using namespace car::system::movement::devices;

namespace car::system::movement::controller
{
    [[nodiscard]] DeviceMovementController::DeviceMovementController()
    {
        this->pwm = std::make_shared<PCA9685>();
        this->rear_left_wheel_ = std::make_unique<RearWheel>(
            this->pwm,
            std::make_unique<TB6612>(Motor_B, PWM_B));
        this->rear_right_wheel_ = std::make_unique<RearWheel>(
            this->pwm,
            std::make_unique<TB6612>(Motor_A, PWM_A));
        this->front_wheels_ = std::make_unique<Servo>(
            this->pwm,
            0);
        this->camera_servo_1_ = std::make_unique<Servo>(
            this->pwm,
            1);
        this->camera_servo_2_ = std::make_unique<Servo>(
            this->pwm,
            2);
    };

    void DeviceMovementController::initialize()
    {
        this->pwm->init(BUS_NUMBER, 0x40);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        this->pwm->setPWMFreq(FREQUENCY);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    void DeviceMovementController::stop()
    {
        spdlog::info("Stopping all devices");
        this->front_wheels_->reset();
        this->camera_servo_1_->reset();
        this->camera_servo_2_->reset();
        this->rear_left_wheel_->stop();
        this->rear_right_wheel_->stop();
    }

    void DeviceMovementController::terminate()
    {
        this->stop();
        spdlog::info("Terminating all devices");
        this->pwm->reset();
        gpioTerminate();
    }

    void DeviceMovementController::setRearWheelsSpeed(const int &speed)
    {
        spdlog::info("Both Rear Wheels speed are set to {}", speed);
        this->rear_left_wheel_->setSpeed(speed);
        this->rear_right_wheel_->setSpeed(speed);
    }

    void DeviceMovementController::setRearLeftWheelSpeed(const int &speed)
    {
        spdlog::info("Left Rear Wheel speed is set to {}", speed);
        this->rear_left_wheel_->setSpeed(speed);
    }

    void DeviceMovementController::setRearRightWheelSpeed(const int &speed)
    {
        spdlog::info("Right Rear Wheel speed is set to {}", speed);
        this->rear_right_wheel_->setSpeed(speed);
    }

    void DeviceMovementController::setFrontWheelsAngle(const float &angle)
    {
        spdlog::info("Front Wheels angle is set to {}", angle);
        this->front_wheels_->setAngle(angle);
    }

    void DeviceMovementController::setCameraServo1Angle(const float &angle)
    {
        spdlog::info("Camera Servo 1 angle is set to {}", angle);
        this->camera_servo_1_->setAngle(angle);
    }

    void DeviceMovementController::setCameraServo2Angle(const float &angle)
    {
        spdlog::info("Camera Servo 2 angle is set to {}", angle);
        this->camera_servo_2_->setAngle(angle);
    }

    void DeviceMovementController::setRearWheelsDirectionToForward()
    {
        spdlog::info("Both Rear Wheels are set to move forward");
        this->rear_left_wheel_->forward();
        this->rear_right_wheel_->forward();
    }

    void DeviceMovementController::setRearLeftWheelDirectionToForward()
    {
        spdlog::info("Left Rear Wheel is set to move forward");
        this->rear_left_wheel_->forward();
    }

    void DeviceMovementController::setRearRightWheelDirectionToForward()
    {
        spdlog::info("Right Rear Wheel is set to move forward");
        this->rear_right_wheel_->forward();
    }

    void DeviceMovementController::setRearWheelsDirectionToBackward()
    {
        spdlog::info("Both Rear Wheels are set to move backward");
        this->rear_left_wheel_->backward();
        this->rear_right_wheel_->backward();
    }

    void DeviceMovementController::setRearLeftWheelDirectionToBackward()
    {
        spdlog::info("Left Rear Wheel is set to move backward");
        this->rear_left_wheel_->backward();
    }

    void DeviceMovementController::setRearRightWheelDirectionToBackward()
    {
        spdlog::info("Right Rear Wheel is set to move backward");
        this->rear_right_wheel_->backward();
    }
} // namespace car::system::movement::controller

#endif // __linux__