#ifndef ABSTRACTWHEELCONTROLLER_CXX
#define ABSTRACTWHEELCONTROLLER_CXX

#pragma once

namespace car::system::movement::controller
{
    class AbstractWheelController
    {
    public:
        virtual void initialize() = 0;

        virtual void setRearWheelsSpeed(const int& speed) = 0;
        virtual void setFrontWheelsAngle(const float& angle) = 0;

        virtual void setRearLeftWheelSpeed(const int& speed) = 0;
        virtual void setRearRightWheelSpeed(const int& speed) = 0;

        virtual void setFrontLeftWheelAngle(const float& angle) = 0;
        virtual void setFrontRightWheelAngle(const float& angle) = 0;

        virtual void setRearWheelDirectionToForwards() = 0;
        virtual void setRearWheelDirectionToBackwards() = 0;
    };
} // namespace car::system::movement::controller

#endif