#ifdef __linux__
#include "car/system/movement/controller/DeviceMovementController.h"

#include <memory>
#include <thread>
#include <chrono>

#include <PCA9685.h>

#include "AbstractMovementController.h"

#include "car/system/movement/devices/Servo.h"
#include "car/system/movement/devices/RearWheel.h"

using namespace car::system::movement::devices;

namespace car::system::movement::controller
{
    [[nodiscard]] DeviceMovementController::DeviceMovementController()
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

    void DeviceMovementController::initialize() final override
    {
        this->pwm->init(BUS_NUMBER, 0x40);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        this->pwm->setPWMFreq(FREQUENCY);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    void DeviceMovementController::stop() final override
    {
        spdlog::info("Stopping all devices");
        this->front_wheels->reset();
        this->camera_servo_1->reset();
        this->camera_servo_2->reset();
        this->rear_left_wheel->stop();
        this->rear_right_wheel->stop();
    }

    void DeviceMovementController::terminate() final override
    {
        this->stop();
        spdlog::info("Terminating all devices");
        this->pwm->reset();
        gpioTerminate();
    }

    void DeviceMovementController::setRearWheelsSpeed(const int &speed) final override
    {
        spdlog::info("Both Rear Wheels speed are set to {}", speed);
        this->rear_left_wheel->setSpeed(speed);
        this->rear_right_wheel->setSpeed(speed);
    }

    void DeviceMovementController::setRearLeftWheelSpeed(const int &speed) final override
    {
        spdlog::info("Left Rear Wheel speed is set to {}", speed);
        this->rear_left_wheel->setSpeed(speed);
    }

    void DeviceMovementController::setRearRightWheelSpeed(const int &speed) final override
    {
        spdlog::info("Right Rear Wheel speed is set to {}", speed);
        this->rear_right_wheel->setSpeed(speed);
    }

    void DeviceMovementController::setFrontWheelsAngle(const float &angle) final override
    {
        spdlog::info("Front Wheels angle is set to {}", angle);
        this->front_wheels->setAngle(angle);
    }

    void DeviceMovementController::setCameraServo1Angle(const float &angle) final override
    {
        spdlog::info("Camera Servo 1 angle is set to {}", angle);
        this->camera_servo_1->setAngle(angle);
    }

    void DeviceMovementController::setCameraServo2Angle(const float &angle) final override
    {
        spdlog::info("Camera Servo 2 angle is set to {}", angle);
        this->camera_servo_2->setAngle(angle);
    }

    void DeviceMovementController::setRearWheelsDirectionToForward() final override
    {
        spdlog::info("Both Rear Wheels are set to move forward");
        this->rear_left_wheel->forward();
        this->rear_right_wheel->forward();
    }

    void DeviceMovementController::setRearLeftWheelDirectionToForward() final override
    {
        spdlog::info("Left Rear Wheel is set to move forward");
        this->rear_left_wheel->forward();
    }

    void DeviceMovementController::setRearRightWheelDirectionToForward() final override
    {
        spdlog::info("Right Rear Wheel is set to move forward");
        this->rear_right_wheel->forward();
    }

    void DeviceMovementController::setRearWheelsDirectionToBackward() final override
    {
        spdlog::info("Both Rear Wheels are set to move backward");
        this->rear_left_wheel->backward();
        this->rear_right_wheel->backward();
    }

    void DeviceMovementController::setRearLeftWheelDirectionToBackward() final override
    {
        spdlog::info("Left Rear Wheel is set to move backward");
        this->rear_left_wheel->backward();
    }

    void DeviceMovementController::setRearRightWheelDirectionToBackward() final override
    {
        spdlog::info("Right Rear Wheel is set to move backward");
        this->rear_right_wheel->backward();
    }
} // namespace car::system::movement::controller

#endif // __linux__