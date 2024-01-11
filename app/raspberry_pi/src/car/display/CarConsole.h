#ifndef CARCONSOLE_H
#define CARCONSOLE_H

#pragma once

#include "CarConsole.h"

#include <memory>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/loop.hpp> // ftxui::Loop

#include <nod/nod.hpp>

#include "../system/CarSystem.h"
#include "component/DebugWheelRenderer.cxx"

using namespace car::system;
using namespace ftxui;
using namespace car::display::component;

namespace car::display {
	static const ButtonOption animated_button_style = ButtonOption::Animated();
	class CarConsole
	{
	public:
		inline Component MainComponent(Component main_button, std::function<void()> show_exit_modal);
		inline Component ExitModalComponent(std::function<void()> hide_exit_modal, std::function<void()> exit);
		inline Component DebugEnableWarningComponent(std::function<void()> hide_enable_debug_warning_modal, std::function<void()> enable_debug_mode);

		CarConsole(std::unique_ptr<CarSystem> car_system);

		void initialize();

		void run();

	private:
		std::unique_ptr<CarSystem> car_system;
	};
}


#endif