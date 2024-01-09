#ifndef ABSTRACTWHEELCONTROLLER_CXX
#define ABSTRACTWHEELCONTROLLER_CXX

#pragma once

#include "../../messaging/commands/MoveCommand.cxx"
#include "../../messaging/commands/TurnCommand.cxx"

using namespace car::system::messaging::commands;

namespace car::system::movement::controller
{
    class AbstractWheelController
    {
    public:
        virtual void initialize() = 0;

        virtual void set_rear_wheels_speed(const SpeedCommand &speed_command) = 0;
        virtual void set_front_wheels_angle(const TurnCommand &angle_command) = 0;

        virtual void set_rear_left_wheel_speed(const SpeedCommand& speed_command) = 0;
        virtual void set_rear_right_wheel_speed(const SpeedCommand& speed_command) = 0;

        virtual void set_front_left_wheel_angle(const TurnCommand& angle_command) = 0;
        virtual void set_front_right_wheel_angle(const TurnCommand& angle_command) = 0;
    };
} // namespace car::system::movement::controller

#endif