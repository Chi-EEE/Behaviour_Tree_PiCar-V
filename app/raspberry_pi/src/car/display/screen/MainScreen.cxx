#ifndef MAINSCREEN_CXX
#define MAINSCREEN_CXX

#pragma once

#include <memory>

#include <ftxui/component/component.hpp>

#include "../../system/CarSystem.h"

#include "../component/main/MainButton.cxx"
#include "../component/main/MainExitModal.cxx"

using namespace car::system;
using namespace car::display::component::main;

using namespace ftxui;

namespace car::display::screen
{
	class MainScreen
	{
	public:
		MainScreen(std::shared_ptr<CarSystem> car_system, std::function<void()> exit) : car_system(car_system), main_button(car_system, box), main_exit_modal(exit)
		{
			auto main_button_component = main_button.element();

			auto main_exit_modal_component = main_exit_modal.element();

			auto show_exit_modal = [&]
			{ main_exit_modal.exit_modal_shown = true; };

			this->left_side =
				Container::Vertical({
					Renderer([&]()
							 {
						// TODO: Fix the y size of the canvas
						auto c = Canvas(box.x_max, box.y_max * 3);
						for (auto& point : this->car_system->getScanData()) {
							const double angle = point.angle;
							const double distance = point.distance;
							const double angleInRadians = angle * (3.14159265f / 180.0f);
							const int x = distance * std::cos(angleInRadians);
							const int y = distance * std::sin(angleInRadians);
							c.DrawBlockLine(50, 50, x, y, Color::Blue);
						}
						return canvas(std::move(c)); }),
					Renderer([&]
							 { return separator(); }),
					Container::Horizontal({
						main_button_component | center,
						Renderer([&]
								 { return separator(); }),
						Button("Quit", std::move(show_exit_modal), ButtonOption::Animated()) | center,
					}),
				}) |
				xflex;

			this->b = Button(
				"Start", [&] {}, ButtonOption::Animated());
			this->right_side = Renderer(Container::Vertical({this->b}), [&]
										{ return vbox({
											  text("Behaviour Tree Queue"),
											  separator(),
											  vbox({
												  hbox({
													  text("Behaviour Tree: ") | vcenter,
													  separator(),
													  this->b->Render(),
												  }) | border,
											  }),
										  }); });

			this->main_component = Container::Horizontal(
									   {
										   left_side,
										   Renderer([&]
													{ return separator(); }),
										   right_side,
									   }) |
								   border | flex | reflect(box);

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

		Component b;

		Component left_side;
		Component right_side;

		Component main_component;
	};
}

#endif