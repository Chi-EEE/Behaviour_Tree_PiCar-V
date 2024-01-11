#ifndef SETTINGSSCREEN_CXX
#define SETTINGSSCREEN_CXX

#pragma once

#include <memory>

#include <ftxui/component/component.hpp>

#include "../../system/CarSystem.h"

#include "../component/debug/DebugEnabler.cxx"
#include "../component/debug/DebugLidarCheckbox.cxx"
#include "../component/debug/DebugWheelRenderer.cxx"

using namespace car::system;
using namespace car::display::component::debug;

using namespace ftxui;

namespace car::display::screen {
	class SettingsScreen {
	public:
		SettingsScreen(std::shared_ptr<CarSystem> car_system): car_system(car_system) {

		}

		Component element() {
			this->debug_lidar_checkbox.getLidarMotorSignal().connect([&](bool connected)
				{
					if (connected) {
						this->car_system->startLidarDevice();
					}
					else {
						this->car_system->stopLidarDevice();
					}
				}
			);

			auto lidar_motor_checkbox_component = this->debug_lidar_checkbox.element();

			this->debug_wheel_renderer.getRearWheelDirectionSignal().connect([&](bool direction)
				{
					if (direction) {
						this->car_system->setRearWheelDirectionToForwards();
					}
					else {
						this->car_system->setRearWheelDirectionToBackwards();
					}
				}
			);

			auto wheel_settings_renderer = this->debug_wheel_renderer.element();

			auto settings_container = Container::Vertical(
				{
					this->debug_enabler.getCheckbox(),
					Container::Vertical(
						{
							lidar_motor_checkbox_component,
							Renderer([&] {return separator(); }),
							wheel_settings_renderer,
						}
					) | border | Maybe(&this->debug_enabler.isEnabled())
				}
			) | border;

			settings_container |= this->debug_enabler.getWarningModal();

			return settings_container;
		}

		void update() {
			if (this->debug_enabler.isEnabled()) {
				if (this->debug_wheel_renderer.updateFrontWheels()) {
					this->car_system->setFrontLeftWheelAngle({ this->debug_wheel_renderer.getFrontLeftWheelAngleSliderValue() * 1.0f });
					this->car_system->setFrontRightWheelAngle({ this->debug_wheel_renderer.getFrontRightWheelAngleSliderValue() * 1.0f });
				}
				if (this->debug_wheel_renderer.updateRearWheels()) {
					this->car_system->setRearLeftWheelSpeed({ this->debug_wheel_renderer.getRearLeftWheelSpeedSliderValue() });
					this->car_system->setRearRightWheelSpeed({ this->debug_wheel_renderer.getRearRightWheelSpeedSliderValue() });
				}
			}
		}

	private:
		std::shared_ptr<CarSystem> car_system;
	
		DebugEnabler debug_enabler;
		DebugLidarCheckbox debug_lidar_checkbox;
		DebugWheelRenderer debug_wheel_renderer;
	};
}

#endif