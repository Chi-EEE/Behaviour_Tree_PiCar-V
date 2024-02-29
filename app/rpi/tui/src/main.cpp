#include <optional>
#include <string>
#include <thread>
#include <chrono>

#ifdef __linux
#include <unistd.h>
#endif

#include <cxxopts.hpp>

#include <fmt/format.h>

#include "car/configuration/Configuration.h"

#include "car/display/console/CarConsole.h"

#include "car/system/CarSystem.h"

#include "car/system/lidar/LidarScanner.h"
#include "car/system/lidar/LidarDummy.h"

#include "car/system/movement/controller/DummyMovementController.h"
#include "car/system/movement/controller/DeviceMovementController.h"

#include "car/plugin/PluginManager.h"

#include "car/system/logging/VectorSink.h"

#include "behaviour_tree/BehaviourTreeHandler.hpp"

using namespace car::configuration;
using namespace car::plugin;
using namespace car::display::console;
using namespace car::system;
using namespace car::system::lidar;
using namespace car::system::messaging;
using namespace car::system::movement::controller;
using namespace behaviour_tree;
using namespace rplidar;

std::unique_ptr<LidarDevice> getLidarDevice(bool dummy) {
	if (dummy) {
		return std::make_unique<LidarDummy>();
	}
	else {
#ifdef __linux
		auto maybe_scanner = LidarScanner::create("/dev/ttyUSB0");
#else
		auto maybe_scanner = LidarScanner::create("COM3");
#endif
		if (!maybe_scanner.has_value())
		{
			spdlog::error("Unable to connect to the Lidar Scanner");
			throw std::runtime_error("Unable to connect to the Lidar Scanner");
		}
		return std::move(maybe_scanner.value());
	}
}

int main(int argc, char* argv[])
{
#ifdef __linux
	if (!getuid()) {
		std::cout << "This program will not work properly unless you are root. Please run this program as root using `sudo`.\n";
		return EXIT_FAILURE;
	}
#endif
	cxxopts::Options options("Route Navigator", "Program to run the Raspberry Pi Application");

	options.add_options()
		("disable-console-display", "Disable Console Display", cxxopts::value<bool>()->default_value("false"))
		("ip-address", "Set IP Address for Raspberry Pi to listen to", cxxopts::value<std::string>())
		("port", "Set Port of the IP Address for Raspberry Pi to listen to", cxxopts::value<std::string>());

	auto result = options.parse(argc, argv);

	std::string exe_dir = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path().string();

	std::shared_ptr<Configuration> configuration = std::make_shared<Configuration>(exe_dir);
	configuration->setConfigFilePath("settings/config.jsonc");
	configuration->load();

	std::unique_ptr<LidarDevice> scanner = getLidarDevice(true);

	std::unique_ptr<MessagingSystem> messaging_system = std::make_unique<MessagingSystem>(configuration);

#ifdef __linux
	// std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(std::make_unique<DeviceMovementController>());
	std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(std::make_unique<DummyMovementController>());
#else
	std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(std::make_unique<DummyMovementController>());
#endif

	std::shared_ptr<BehaviourTreeHandler> behaviour_tree_handler = std::make_shared<BehaviourTreeHandler>();

	std::unique_ptr<PluginManager> plugin_manager = std::make_unique<PluginManager>();
	plugin_manager->addPlugin(behaviour_tree_handler);

	std::shared_ptr<CarSystem> car_system = std::make_shared<CarSystem>(
		configuration,
		std::move(scanner),
		std::move(messaging_system),
		std::move(movement_system),
		std::move(plugin_manager));

	bool disable_console_display = result["disable-console-display"].as<bool>();
	if (!disable_console_display)
	{
		std::shared_ptr<car::system::logging::vector_sink_mt> vector_sink = std::make_shared<car::system::logging::vector_sink_mt>(300);
		auto vector_sink_logger = std::make_shared<spdlog::logger>("CLI", static_cast<std::shared_ptr<spdlog::sinks::sink>>(vector_sink));
		spdlog::set_default_logger(vector_sink_logger);

		// The CarConsole object will display the UI and handle user input:
		CarConsole car_console(std::move(car_system), vector_sink);
		car_console.initialize();
		car_console.run();
		car_console.terminate();
	}
	else {
		spdlog::warn("Console Display is disabled. Exiting the Program during execution may leave devices running!");
		do {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			car_system->update();
		} while (true);
	}
	return EXIT_SUCCESS;
	}
