#ifndef MAINERRORMODAL_CXX
#define MAINERRORMODAL_CXX

#pragma once

#include <ftxui/component/component.hpp>

#include "car/system/CarSystem.h"

using namespace ftxui;
using namespace car::system;

namespace car::display::console::component::main
{
	class MainErrorModal
	{
	public:
		MainErrorModal() {}
		Component element()
		{
			auto hide_exit_modal = [&]
				{ 
					this->exit_modal_shown = false;
				};

			this->error_element = text("Error: Something went wrong. Please try again later.");

			this->main_error_modal = Container::Vertical({
				Button("Ok", std::move(hide_exit_modal), ButtonOption::Animated()),
				});

			this->main_error_modal |= Renderer([&](Element inner)
				{ return vbox({
							  this->error_element,
							 separator(),
							 inner,
					}) |
					size(WIDTH, GREATER_THAN, 30) | border; });

			return this->main_error_modal;
		}

		void setErrorMessage(std::string message)
		{
			this->error_element = text("Error: " + std::move(message));
		}

		bool exit_modal_shown = false;

	private:
		Component main_error_modal;
		Element error_element;
	};
}

#endif