#ifndef CONNECTBUTTON_CXX
#define CONNECTBUTTON_CXX

#pragma once

#include <ftxui/component/component.hpp>

#include "car/system/CarSystem.h"

using namespace ftxui;
using namespace car::system;

namespace car::display::console::component::main
{
	class ConnectButton
	{
	public:
		ConnectButton(std::shared_ptr<CarSystem> car_system, Box& box) : car_system(car_system)
		{
			auto connect_button_lambda = [&]
				{
					if (this->main_debounce)
						return;
					this->main_debounce = true;
					this->button_pressed = !this->button_pressed;
					if (this->button_pressed)
					{
						this->main_button_text = "Attempting to connect to Websocket Server...";
						auto connection_result = this->car_system->connect();
						if (!connection_result.has_value()) {
							this->button_pressed = false;
							this->main_button_text = "Start Car Application";
							this->on_connect_failure(connection_result.error());
						}
						else {
							this->main_button_text = "Stop Car Application";
						}
					}
					else
					{
						this->main_button_text = "Disconnecting from the Websocket Server...";
						this->car_system->disconnect();
						this->main_button_text = "Start Car Application";
					}
					this->main_debounce = false;
				};

			this->main_button = Button(&this->main_button_text, std::move(connect_button_lambda), ButtonOption::Animated());
		}

		Component element()
		{
			return this->main_button;
		}

		std::function<void(std::string)> on_connect_failure = [](std::string _) {};

	private:
		std::shared_ptr<CarSystem> car_system;

		bool main_debounce = false;
		bool button_pressed = false;
		std::string main_button_text = "Start Car Application";

		Component main_button;
	};
}

#endif