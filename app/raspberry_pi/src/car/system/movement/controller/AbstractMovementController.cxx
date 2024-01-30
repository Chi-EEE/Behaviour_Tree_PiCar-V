#ifndef ABSTRACTWHEELCONTROLLER_CXX
#define ABSTRACTWHEELCONTROLLER_CXX

#pragma once

namespace car::system::movement::controller
{
    class AbstractMovementController
    {
    public:
        virtual void initialize() = 0;
        virtual void stop() = 0;
        virtual void terminate() = 0;

        virtual void setRearWheelsSpeed(const int &speed) = 0;

        virtual void setRearLeftWheelSpeed(const int &speed) = 0;
        virtual void setRearRightWheelSpeed(const int &speed) = 0;

        virtual void setFrontWheelsAngle(const float &angle) = 0;
        virtual void setCameraServo1Angle(const float &angle) = 0;
        virtual void setCameraServo2Angle(const float &angle) = 0;

        virtual void setRearWheelsDirectionToForward() = 0;
        virtual void setRearLeftWheelDirectionToForward() = 0;
        virtual void setRearRightWheelDirectionToForward() = 0;

        virtual void setRearWheelsDirectionToBackward() = 0;
        virtual void setRearLeftWheelDirectionToBackward() = 0;
        virtual void setRearRightWheelDirectionToBackward() = 0;
    };
} // namespace car::system::movement::controller

#endif