#ifndef MAINEXITMODAL_CXX
#define MAINEXITMODAL_CXX

#pragma once

#include <ftxui/component/component.hpp>

#include "../../../system/CarSystem.h"

using namespace ftxui;
using namespace car::system;

namespace car::display::component::main {
	class MainExitModal {
	public:
		MainExitModal(std::function<void()> exit) : exit(exit) {}
		Component element() {
			auto hide_exit_modal = [&] { this->exit_modal_shown = false; };

			this->main_exit_modal = Container::Vertical({
				Button("No", std::move(hide_exit_modal), ButtonOption::Animated()),
				Button("Yes", exit, ButtonOption::Animated()),
				}
			);

			this->main_exit_modal |= Renderer([&](Element inner) {
				return vbox({
						   text("Are you sure you want to exit?"),
						   separator(),
						   inner,
					})
					| size(WIDTH, GREATER_THAN, 30)
					| border;
				}
			);

			return this->main_exit_modal;
		}

		bool exit_modal_shown = false;
	private:
		std::function<void()> exit;

		Component main_exit_modal;
	};
}

#endif