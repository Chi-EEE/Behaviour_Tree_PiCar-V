#ifndef MAINSCREEN_CXX
#define MAINSCREEN_CXX

#pragma once

#include <memory>

#include <ftxui/component/component.hpp>

#include "car/system/CarSystem.h"

#include "../component/main/ConnectButton.cxx"
#include "../component/main/MainExitModal.cxx"
#include "../component/main/MainErrorModal.cxx"

using namespace car::system;
using namespace car::display::console::component::main;

using namespace ftxui;

namespace car::display::console::screen
{
	class MainScreen
	{
	public:
		MainScreen(std::shared_ptr<CarSystem> car_system, std::function<void()> exit) : car_system(car_system), connect_button(car_system, box), main_exit_modal(exit), main_error_modal()
		{
			auto connect_button_component = this->connect_button.element();

			auto main_exit_modal_component = this->main_exit_modal.element();
			auto main_error_modal_component = this->main_error_modal.element();

			auto show_exit_modal = [&]
				{ this->main_exit_modal.exit_modal_shown = true; };

			std::function<void(std::string)> on_connect_failure = [&](std::string error_message) {
				this->main_error_modal.setErrorMessage(error_message);
				this->main_error_modal.error_modal_shown = true;
				};

			this->connect_button.on_connect_failure = on_connect_failure;

			this->car_system->getMessagingSystem()->getDisconnectSignal().connect([&](std::string disconnect_message) {
				spdlog::info(disconnect_message);
				});

			this->main_screen =
				Container::Vertical({
					Renderer([&]()
							 {
							auto c = Canvas(this->box.x_max, this->box.y_max * 3);
							std::vector<rplidar::Measure> scan_data = this->car_system->getDeviceManager()->getLidarDevice()->getScanData();
							for (auto& point : scan_data) {
								const double angle = point.angle;
								const double distance = point.distance;
								const double angleInRadians = angle * (3.14159265f / 180.0f);
								const int x = distance * std::cos(angleInRadians);
								const int y = distance * std::sin(angleInRadians);
								c.DrawBlockLine(50, 50, x, y, Color::Blue);
							}
							return canvas(std::move(c)); }) | center,
						Renderer([&]
								 { return separator(); }),
						Container::Horizontal({
							connect_button_component | flex,
							Renderer([&]
									 { return separator(); }),
							Button("Quit", std::move(show_exit_modal), ButtonOption::Animated()) | flex,
						}) | center,
					}) |
				xflex;

							this->main_component = this->main_screen | border | flex | reflect(this->box);

							this->main_component |= Modal(main_exit_modal_component, &main_exit_modal.exit_modal_shown);
							this->main_component |= Modal(main_error_modal_component, &main_error_modal.error_modal_shown);
		}

		Component element()
		{
			return this->main_component;
		}

	private:
		std::shared_ptr<CarSystem> car_system;

		Box box;

		ConnectButton connect_button;
		MainExitModal main_exit_modal;
		MainErrorModal main_error_modal;

		Component info;

		Component main_screen;

		Component main_component;
	};
}

#endif
