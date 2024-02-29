#ifndef DEBUGLIDARCHECKBOX_CXX
#define DEBUGLIDARCHECKBOX_CXX

#pragma once

#include <nod/nod.hpp>

#include <ftxui/component/component.hpp>

using namespace ftxui;

namespace car::display::console::component::debug
{
	class DebugLidarCheckbox
	{
	private:
		static constexpr auto LIDAR_MOTOR_ENABLED_MESSAGE = "Lidar Motor Status: Enabled";
		static constexpr auto LIDAR_MOTOR_DISABLED_MESSAGE = "Lidar Motor Status: Disconnected";

	public:
		DebugLidarCheckbox()
		{
			auto lidar_motor_checkbox_option = CheckboxOption::Simple();
			lidar_motor_checkbox_option.on_change = [&]
			{
				if (this->lidar_motor_loading_debounce)
				{
					this->lidar_motor_enabled = !this->lidar_motor_enabled;
					return;
				}
				this->lidar_motor_loading_debounce = true;
				if (this->lidar_motor_enabled)
				{
					this->lidar_motor_status = LIDAR_MOTOR_ENABLED_MESSAGE;
				}
				else
				{
					this->lidar_motor_status = LIDAR_MOTOR_DISABLED_MESSAGE;
				}
				this->lidar_motor_signal(this->lidar_motor_enabled);
				this->lidar_motor_loading_debounce = false;
			};

			this->lidar_motor_checkbox_component = Checkbox(&this->lidar_motor_status, &this->lidar_motor_enabled, lidar_motor_checkbox_option);
		}

		Component element()
		{
			return this->lidar_motor_checkbox_component;
		}

		nod::signal<void(bool)> &getLidarMotorSignal()
		{
			return this->lidar_motor_signal;
		}

	private:
		nod::signal<void(bool)> lidar_motor_signal;

		std::string lidar_motor_status = LIDAR_MOTOR_DISABLED_MESSAGE;
		bool lidar_motor_loading_debounce = false;
		bool lidar_motor_enabled = false;

		Component lidar_motor_checkbox_component;
	};
}

#endif