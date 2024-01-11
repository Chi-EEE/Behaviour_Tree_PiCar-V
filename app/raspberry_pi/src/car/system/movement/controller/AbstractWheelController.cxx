#ifndef ABSTRACTWHEELCONTROLLER_CXX
#define ABSTRACTWHEELCONTROLLER_CXX

#pragma once

#include "../../messaging/commands/SpeedCommand.cxx"
#include "../../messaging/commands/AngleCommand.cxx"

using namespace car::system::messaging::commands;

namespace car::system::movement::controller
{
    class AbstractWheelController
    {
    public:
        virtual void initialize() = 0;

        virtual void setRearWheelsSpeed(const SpeedCommand &speed_command) = 0;
        virtual void setFrontWheelsAngle(const AngleCommand &angle_command) = 0;

        virtual void setRearLeftWheelSpeed(const SpeedCommand& speed_command) = 0;
        virtual void setRearRightWheelSpeed(const SpeedCommand& speed_command) = 0;

        virtual void setFrontLeftWheelAngle(const AngleCommand& angle_command) = 0;
        virtual void setFrontRightWheelAngle(const AngleCommand& angle_command) = 0;

        virtual void setRearWheelDirectionToForwards() = 0;
        virtual void setRearWheelDirectionToBackwards() = 0;
    };
} // namespace car::system::movement::controller

#endif