#include "car/system/movement/controller/DummyMovementController.h"

#include <spdlog/spdlog.h>

namespace car::system::movement::controller
{
    void DummyMovementController::stop()
    {
        spdlog::info("[DUMMY] Car is stopped");
    }

    void DummyMovementController::setRearWheelsSpeed(const int speed)
    {
        spdlog::info("[DUMMY] Both Rear Wheels speed are set to {}", speed);
    }

    void DummyMovementController::setRearLeftWheelSpeed(const int speed)
    {
        spdlog::info("[DUMMY] Left Rear Wheel speed is set to {}", speed);
    }

    void DummyMovementController::setRearRightWheelSpeed(const int speed)
    {
        spdlog::info("[DUMMY] Right Rear Wheel speed is set to {}", speed);
    }

    void DummyMovementController::setFrontWheelsAngle(const float angle)
    {
        spdlog::info("[DUMMY] Front Wheels angle is set to {}", angle);
    }

    void DummyMovementController::setCameraServo1Angle(const float angle)
    {
        spdlog::info("[DUMMY] Camera Servo 1 angle is set to {}", angle);
    }

    void DummyMovementController::setCameraServo2Angle(const float angle)
    {
        spdlog::info("[DUMMY] Camera Servo 2 angle is set to {}", angle);
    }

    void DummyMovementController::setRearWheelsDirectionToForward()
    {
        spdlog::info("[DUMMY] Both Rear Wheels are set to move forward");
    }

    void DummyMovementController::setRearLeftWheelDirectionToForward()
    {
        spdlog::info("[DUMMY] Left Rear Wheel is set to move forward");
    }

    void DummyMovementController::setRearRightWheelDirectionToForward()
    {
        spdlog::info("[DUMMY] Right Rear Wheel is set to move forward");
    }

    void DummyMovementController::setRearWheelsDirectionToBackward()
    {
        spdlog::info("[DUMMY] Both Rear Wheels are set to move backward");
    }

    void DummyMovementController::setRearLeftWheelDirectionToBackward()
    {
        spdlog::info("[DUMMY] Left Rear Wheel is set to move backward");
    }

    void DummyMovementController::setRearRightWheelDirectionToBackward()
    {
        spdlog::info("[DUMMY] Right Rear Wheel is set to move backward");
    }
} // namespace car::system::movement::controller

