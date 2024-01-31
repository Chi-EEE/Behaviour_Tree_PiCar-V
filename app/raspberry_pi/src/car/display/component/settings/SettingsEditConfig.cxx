#ifndef SETTINGSEDITCONFIG_CXX
#define SETTINGSEDITCONFIG_CXX

#pragma once

#include <ftxui/component/component.hpp>
// TODO: Fix this
#include "../../../../car/system/CarSystem.h"

using namespace ftxui;

namespace car::display::component::settings
{
	class SettingsEditConfig
	{
	public:
		SettingsEditConfig(std::shared_ptr<system::CarSystem> car_system) : car_system(car_system) {
		}

		Component element()
		{
			InputOption option;
			option.on_change = [&]()
				{
					this->car_system->setConfigurationPath(this->settings_file_path);
				};
			this->placeholder = "settings/config.jsonc";
			this->input_settings_file_path = Input(&this->settings_file_path, &this->placeholder, option);
			this->load_button = Button("Load", [&]()
				{
					auto result = this->car_system->loadConfiguation();
					if (!result.has_value()) {
						this->settings_file_path = "";
						this->placeholder = result.error();
					}
				}
			);

			return Renderer(
				Container::Horizontal({ this->input_settings_file_path, this->load_button, }),
				[&] {
					return hbox(
						{
							this->input_settings_file_path->Render() | border,
							this->load_button->Render() | border,
						}
					);
				}
			);
		}
	private:
		std::shared_ptr<system::CarSystem> car_system;

		std::string placeholder;
		std::string settings_file_path;

		Component input_settings_file_path;
		Component load_button;
	};
}

#endif