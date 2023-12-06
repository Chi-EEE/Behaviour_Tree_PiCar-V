#ifndef CARCONSOLE_H
#define CARCONSOLE_H

#pragma once

#include <thread> // std::this_thread::sleep_for
#include <memory>

#include <nlohmann/json.hpp>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/loop.hpp> // ftxui::Loop

#include "CarSystem.h"

using json = nlohmann::json;

using namespace ftxui;

auto button_style = ButtonOption::Animated();

namespace car_system {
	class CarConsole
	{
	public:
		Component MainComponent(std::function<void()> show_exit_modal) {
			auto component = Container::Vertical({
				Button("Quit", show_exit_modal, button_style),
				});
			// Polish how the two buttons are rendered:
			component |= Renderer([&](Element inner) {
				return vbox({
						   text("Main component"),
						   separator(),
						   inner,
					})                                //
					| size(WIDTH, GREATER_THAN, 15)   //
					| size(HEIGHT, GREATER_THAN, 15)  //
					| border                          //
					| center;                         //
				});
			return component;
		}

		Component ExitModalComponent(std::function<void()> hide_exit_modal,
			std::function<void()> exit) {
			auto component = Container::Vertical({
				Button("No", hide_exit_modal, button_style),
				Button("Yes", exit, button_style),
				});
			// Polish how the two buttons are rendered:
			component |= Renderer([&](Element inner) {
				return vbox({
						   text("Are you sure you want to exit?"),
						   separator(),
						   inner,
					})                               //
					| size(WIDTH, GREATER_THAN, 30)  //
					| border;                        //
				});
			return component;
		}

		CarConsole(std::unique_ptr<CarSystem> car_system_obj) : car_system_obj(std::move(car_system_obj)) {
		};

		void run() {
			auto screen = ScreenInteractive::Fullscreen();

			// State of the application:
			bool exit_modal_shown = false;

			// Some actions modifying the state:
			auto show_exit_modal = [&] { exit_modal_shown = true; };
			auto hide_exit_modal = [&] { exit_modal_shown = false; };
			auto exit = screen.ExitLoopClosure();

			auto main_component = MainComponent(show_exit_modal);
			auto modal_component = ExitModalComponent(hide_exit_modal, exit);
			main_component |= Modal(modal_component, &exit_modal_shown);

			Loop loop(&screen, main_component);

			while (!loop.HasQuitted()) {
				this->car_system_obj->run();
				loop.RunOnce();
			}
			this->car_system_obj->terminate();
		};

	private:
		std::unique_ptr<CarSystem> car_system_obj;
	};
}

#endif