#ifndef MAINSCREEN_CXX
#define MAINSCREEN_CXX

#pragma once

#include <memory>

#include <ftxui/component/component.hpp>

#include "../src/car/system/CarSystem.h"

#include "../component/main/MainButton.cxx"
#include "../component/main/MainExitModal.cxx"

using namespace car::system;
using namespace car::display::console::component::main;

using namespace ftxui;

namespace car::display::console::screen
{
	class MainScreen
	{
	public:
		MainScreen(std::shared_ptr<CarSystem> car_system, std::function<void()> exit) : car_system(car_system), main_button(car_system, box), main_exit_modal(exit)
		{
			auto main_button_component = this->main_button.element();

			auto main_exit_modal_component = this->main_exit_modal.element();

			auto show_exit_modal = [&]
				{ this->main_exit_modal.exit_modal_shown = true; };

			this->main_screen =
				Container::Vertical({
					Renderer([&]()
							 {
							auto c = Canvas(this->box.x_max, this->box.y_max * 3);
							std::vector<rplidar::Measure> scan_data = this->car_system->getScanData();
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
							main_button_component | flex,
							Renderer([&]
									 { return separator(); }),
							Button("Quit", std::move(show_exit_modal), ButtonOption::Animated()) | flex,
						}) | center,
					}) |
				xflex;

			this->main_component = this->main_screen | border | flex | reflect(this->box);

			this->main_component |= Modal(main_exit_modal_component, &main_exit_modal.exit_modal_shown);
		}

		Component element()
		{
			return this->main_component;
		}

	private:
		std::shared_ptr<CarSystem> car_system;

		Box box;

		MainButton main_button;
		MainExitModal main_exit_modal;

		Component info;

		Component main_screen;

		Component main_component;
	};
}

#endif