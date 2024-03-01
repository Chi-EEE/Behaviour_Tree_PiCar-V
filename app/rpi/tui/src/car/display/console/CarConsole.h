#ifndef CARCONSOLE_H
#define CARCONSOLE_H

#pragma once

#include <memory>

#include "car/system/CarSystem.h"
#include "car/system/logging/VectorSink.h"

#include "screen/MainScreen.cxx"
#include "screen/SettingsScreen.cxx"
#include "screen/LoggingScreen.cxx"

using namespace car::system;
using namespace car::display::console::screen;

namespace car::display::console
{
	class CarConsole
	{
	public:
		CarConsole(std::shared_ptr<CarSystem> car_system, std::shared_ptr<JsonConfiguration> json_configuration, std::shared_ptr<logging::vector_sink_mt> vector_sink);

		void initialize();

		void run();

		void terminate();

	private:
		std::shared_ptr<CarSystem> car_system;
		std::shared_ptr<JsonConfiguration> json_configuration;
		std::shared_ptr<logging::vector_sink_mt> vector_sink;
	};
}

#endif