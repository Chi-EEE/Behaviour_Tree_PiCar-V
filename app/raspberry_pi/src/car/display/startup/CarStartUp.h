#ifndef CARSTARTUP_H
#define CARSTARTUP_H

#pragma once

#include <memory>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/loop.hpp>

#include <nod/nod.hpp>

using namespace ftxui;

namespace car::display::startup
{
	class CarStartUp
	{
	public:
		CarStartUp();

		void initialize();

		void run();

		void terminate();

	private:
	};
}

#endif