#ifndef SETTINGSSCREEN_CXX
#define SETTINGSSCREEN_CXX

#pragma once

#include <memory>

#include <ftxui/component/component.hpp>

#include "car/system/CarSystem.h"

#include "../../../configuration/JsonConfiguration.cxx"

#include "../component/settings/SettingsEditConfig.cxx"

#include "../component/debug/DebugEnabler.cxx"
#include "../component/debug/DebugLidarCheckbox.cxx"
#include "../component/debug/DebugMovementRenderer.cxx"
#include "../component/debug/DebugMessagingTextbox.cxx"

using namespace car::system;
using namespace car::display::console::component::settings;
using namespace car::display::console::component::debug;

using namespace ftxui;

namespace car::display::console::screen
{
    class SettingsScreen
    {
    public:
        SettingsScreen(std::shared_ptr<CarSystem> car_system, std::shared_ptr<JsonConfiguration> json_configuration) : car_system(car_system),
                                                                debug_messaging_text_box(DebugMessagingTextbox(car_system->getHandleMessageSignal())),
                                                                settings_edit_config(car_system, json_configuration)
        {
        }

        Component element()
        {
            this->debug_lidar_checkbox.getLidarMotorSignal().connect([&](bool connected)
                                                                     {
					if (connected) {
						this->car_system->startLidarDevice();
					}
					else {
						this->car_system->stopLidarDevice();
					} });

            auto lidar_motor_checkbox_component = this->debug_lidar_checkbox.element();

            this->debug_movement_renderer.getRearWheelDirectionSignal().connect([&](bool direction)
                                                                                {
					if (direction) {
						this->car_system->setRearWheelsDirectionToForward();
					}
					else {
						this->car_system->setRearWheelsDirectionToBackward();
					} });

            auto movement_settings_renderer = this->debug_movement_renderer.element();

            auto debug_messaging_text_box_renderer = this->debug_messaging_text_box.element();

            auto settings_container = Container::Vertical(
                {Renderer([&]
                          { return separatorEmpty(); }),
                this->settings_edit_config.element(),
                Container::Vertical({

                }) | borderDouble,
                Container::Vertical({
                    this->debug_enabler.getCheckbox(),
                }) | borderDouble,
                Container::Vertical(
                    {
                        Renderer([&]
                                { return separatorEmpty(); }),
                        Container::Vertical({
                            lidar_motor_checkbox_component,
                        }) | border,
                        Renderer([&]
                                { return separatorEmpty(); }),
                        Container::Vertical({
                            movement_settings_renderer,
                        }) | border,
                        Renderer([&]
                                { return separatorEmpty(); }),
                        Container::Vertical({
                            debug_messaging_text_box_renderer,
                        }) | border,
                    }) |
                    Maybe(&this->debug_enabler.isEnabled())});

            settings_container |= this->debug_enabler.getWarningModal();

            return settings_container;
        }

        void update()
        {
            if (this->debug_enabler.isEnabled())
            {
                if (this->debug_movement_renderer.updateFrontWheels())
                {
                    this->car_system->setFrontWheelsAngle({this->debug_movement_renderer.getFrontWheelsAngleSliderValue() * 1.0f});
                }
                if (this->debug_movement_renderer.updateCameraServo1())
                {
                    this->car_system->setCameraServo1Angle({this->debug_movement_renderer.getCameraServo1AngleSliderValue() * 1.0f});
                }
                if (this->debug_movement_renderer.updateCameraServo2())
                {
                    this->car_system->setCameraServo2Angle({this->debug_movement_renderer.getCameraServo2AngleSliderValue() * 1.0f});
                }
                if (this->debug_movement_renderer.updateRearWheels())
                {
                    this->car_system->setRearLeftWheelSpeed({this->debug_movement_renderer.getRearLeftWheelSpeedSliderValue()});
                    this->car_system->setRearRightWheelSpeed({this->debug_movement_renderer.getRearRightWheelSpeedSliderValue()});
                }
            }
        }

    private:
        std::shared_ptr<CarSystem> car_system;

        SettingsEditConfig settings_edit_config;

        DebugEnabler debug_enabler;
        DebugLidarCheckbox debug_lidar_checkbox;
        DebugMovementRenderer debug_movement_renderer;
        DebugMessagingTextbox debug_messaging_text_box;
    };
}

#endif