#ifndef SETTINGSEDITCONFIG_CXX
#define SETTINGSEDITCONFIG_CXX

#pragma once

#include <ftxui/component/component.hpp>

#include "car/system/CarSystem.h"

#include "../../../../configuration/JsonConfiguration.cxx"

using namespace ftxui;

namespace car::display::console::component::settings
{
	class SettingsEditConfig
	{
	public:
		SettingsEditConfig(std::shared_ptr<system::CarSystem> car_system, std::shared_ptr<JsonConfiguration> json_configuration) : car_system(car_system), json_configuration(json_configuration) {
		}

		Component element()
		{
			InputOption option;
			this->placeholder = "settings/config.jsonc";
			this->input_settings_file_path = Input(&this->settings_file_path, &this->placeholder, option);
			this->load_button = Button("Load", [&]()
				{
					this->json_configuration->setConfigFilePath(this->settings_file_path);
					auto maybe_configuration = this->json_configuration->loadConfiguration();
					if (!maybe_configuration.has_value()) {
						this->car_system->setConfiguration(std::make_shared<Configuration>(maybe_configuration.value()));
					} else {
						this->settings_file_path = "";
						this->placeholder = maybe_configuration.error();
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
		std::shared_ptr<configuration::JsonConfiguration> json_configuration;

		std::string placeholder;
		std::string settings_file_path;

		Component input_settings_file_path;
		Component load_button;
	};
}

#endif