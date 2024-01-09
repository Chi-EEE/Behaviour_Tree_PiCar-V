#ifndef DUMMYWHEELCONTROLLER_CXX
#define DUMMYWHEELCONTROLLER_CXX

#pragma once

#include "AbstractWheelController.cxx"

namespace car::system::movement::controller
{
    class DummyWheelController : public AbstractWheelController
    {
    public:
        void initialize(){};

        void set_rear_wheels_speed(const SpeedCommand &speed_command) override {
        
        }

        void set_front_wheels_angle(const AngleCommand &angle_command) override {

        }

        void set_rear_left_wheel_speed(const SpeedCommand& speed_command) override {

        }

        void set_rear_right_wheel_speed(const SpeedCommand& speed_command) override {

        }

        void set_front_left_wheel_angle(const AngleCommand& angle_command) override {

        }

        void set_front_right_wheel_angle(const AngleCommand& angle_command) override {

        }
    private:
    };
} // namespace car::system::movement::controller

#endif