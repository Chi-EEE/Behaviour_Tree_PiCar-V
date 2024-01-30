#include <optional>
#include <string>

#include <fmt/format.h>

#include "global/Config.hpp"

#include "car/display/CarConsole.h"

#include "car/system/CarSystem.h"

#include "car/system/lidar/LidarScanner.cxx"
#include "car/system/lidar/LidarDummy.cxx"

#include "car/system/movement/controller/DummyMovementController.cxx"
#include "car/system/movement/controller/DeviceMovementController.cxx"

#include "car/plugin/PluginManager.cxx"

#include "car/system/logging/VectorSink.cxx"

#include "behaviour_tree/BehaviourTreeHandler.hpp"

std::string getWebSocketUrl()
{
	std::optional<int> maybe_port = GET_CONFIG_VALUE(port);
	std::string host_name;
	if (maybe_port.has_value())
	{
		host_name = fmt::format("{}:{}", GET_CONFIG_VALUE(host), maybe_port.value());
	}
	else
	{
		host_name = GET_CONFIG_VALUE(host);
	}
	return fmt::format("ws://{}/ws/room?request=join&type=car&room_name={}", host_name, GET_CONFIG_VALUE(room));
}

int main()
{
	using namespace car::plugin;
	using namespace car::display;
	using namespace car::system;
	using namespace car::system::lidar;
	using namespace car::system::messaging;
	using namespace car::system::movement::controller;
	using namespace behaviour_tree;
	using namespace rplidar;

	// spdlog::set_level(spdlog::level::off);

	std::string websocket_url = getWebSocketUrl();

	std::unique_ptr<LidarDummy> scanner = std::make_unique<LidarDummy>();

	//auto maybe_scanner = LidarScanner::create(GET_CONFIG_VALUE(lidar_port));
	//if (!maybe_scanner.has_value())
	//{
	//	spdlog::error("Unable to connect to the Lidar Scanner");
	//	throw std::runtime_error("Unable to connect to the Lidar Scanner");
	//}
	//std::unique_ptr<LidarScanner>& scanner = maybe_scanner.value();

	std::unique_ptr<MessagingSystem> messaging_system = std::make_unique<MessagingSystem>(websocket_url);

	std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(std::make_unique<DummyMovementController>());
	// std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(std::make_unique<DeviceMovementController>());

	std::shared_ptr<BehaviourTreeHandler> behaviour_tree_handler = std::make_shared<BehaviourTreeHandler>();

	std::unique_ptr<PluginManager> plugin_manager = std::make_unique<PluginManager>();
	plugin_manager->addPlugin(behaviour_tree_handler);

	std::shared_ptr<car::system::logging::vector_sink_mt> vector_sink = std::make_shared<car::system::logging::vector_sink_mt>(300);
	auto vector_sink_logger = std::make_shared<spdlog::logger>("CLI", static_cast<std::shared_ptr<spdlog::sinks::sink>>(vector_sink));
	spdlog::set_default_logger(vector_sink_logger);

	std::shared_ptr<CarSystem> car_system = std::make_shared<CarSystem>
		(
			websocket_url,
			std::move(scanner),
			std::move(messaging_system),
			std::move(movement_system),
			std::move(plugin_manager)
		);

	// The CarConsole object will display the UI and handle user input:
	CarConsole car_console(std::move(car_system), vector_sink);
	car_console.initialize();
	car_console.run();
	car_console.terminate();
	return 0;
}
