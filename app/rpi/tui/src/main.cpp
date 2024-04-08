#include <optional>
#include <string>
#include <thread>
#include <chrono>

#ifdef __linux
#include <unistd.h>
#endif

#include <fmt/format.h>

#include "car/display/console/CarConsole.h"

#include "car/configuration/JsonConfiguration.cxx"

#include "car/system/CarSystem.h"

#include "car/system/device/DeviceManager.h"

#include "car/system/device/lidar/LidarDevice.h"
#include "car/system/device/lidar/LidarDummy.h"
#include "car/system/device/lidar/LidarScanner.h"

#include "car/system/device/CameraDevice.h"

#include "car/system/movement/controller/DummyMovementController.h"
#include "car/system/movement/controller/DeviceMovementController.h"

#include "car/plugin/PluginManager.h"

#include "car/system/logging/VectorSink.h"

#include "behaviour_tree/BehaviourTreeHandler.hpp"

using namespace behaviour_tree;
using namespace rplidar;

using namespace car::system;
using namespace car::system::device;
using namespace car::system::device::lidar;
using namespace car::system::messaging;
using namespace car::system::movement::controller;

using namespace car::display::console;

using namespace car::configuration;
using namespace car::plugin;

std::unique_ptr<LidarDevice> getLidarDevice();
std::unique_ptr<AbstractMovementController> getMovementController();

int main(int argc, char* argv[])
{
	std::string exe_dir = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path().string();

	std::shared_ptr<JsonConfiguration> json_configuration = std::make_shared<JsonConfiguration>(exe_dir);
	json_configuration->setConfigFilePath("settings/config.jsonc");

	auto maybe_configuration = json_configuration->loadConfiguration();
	if (!maybe_configuration.has_value())
	{
		spdlog::error("Unable to load the configuration file: {}", maybe_configuration.error());
		return EXIT_FAILURE;
	}

	std::shared_ptr<Configuration> configuration = std::make_shared<Configuration>(maybe_configuration.value());

	auto maybe_camera_device = CameraDevice::create(configuration);
	if (!maybe_camera_device.has_value())
	{
		spdlog::error("Unable to create the camera device: {}", maybe_camera_device.error());
		return EXIT_FAILURE;
	}

	std::unique_ptr<CameraDevice> camera_device = std::move(maybe_camera_device.value());
	std::unique_ptr<LidarDevice> lidar_device = getLidarDevice();

	std::unique_ptr<DeviceManager> device_manager = std::make_unique<DeviceManager>(std::move(camera_device), std::move(lidar_device));

	std::unique_ptr<MessagingSystem> messaging_system = std::make_unique<MessagingSystem>();

	std::unique_ptr<AbstractMovementController> movement_controller = getMovementController();

	std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(std::move(movement_controller));

	std::shared_ptr<BehaviourTreeHandler> behaviour_tree_handler = std::make_shared<BehaviourTreeHandler>();

	std::unique_ptr<PluginManager> plugin_manager = std::make_unique<PluginManager>();
	plugin_manager->addPlugin(behaviour_tree_handler);

	std::shared_ptr<CarSystem> car_system = std::make_shared<CarSystem>(
		configuration,
		std::move(device_manager),
		std::move(messaging_system),
		std::move(movement_system),
		std::move(plugin_manager));

	std::shared_ptr<car::system::logging::vector_sink_mt> vector_sink = std::make_shared<car::system::logging::vector_sink_mt>(300);
	auto vector_sink_logger = std::make_shared<spdlog::logger>("CLI", static_cast<std::shared_ptr<spdlog::sinks::sink>>(vector_sink));
	spdlog::set_default_logger(vector_sink_logger);

	// The CarConsole object will display the UI and handle user input:
	CarConsole car_console(std::move(car_system), std::move(json_configuration), vector_sink);
	car_console.initialize();
	car_console.run();
	car_console.terminate();
	return EXIT_SUCCESS;
}

std::unique_ptr<lidar::LidarDevice> getLidarDevice()
{
	Configuration configuration;
#ifdef __linux
	configuration.lidar_port = "/dev/ttyUSB0";
	auto maybe_scanner = LidarScanner::create(std::make_shared<Configuration>(configuration));
#else
	configuration.lidar_port = "COM3";
	auto maybe_scanner = LidarScanner::create(std::make_shared<Configuration>(configuration));
#endif
	if (maybe_scanner.has_value())
	{
		spdlog::info("Found and using Lidar Scanner\n");
		return std::move(maybe_scanner.value());
	}
	else
	{
		spdlog::warn("Unable to connect to the Lidar Scanner, defaulting to LidarDummy\n");
		return std::make_unique<LidarDummy>();
	}
}

std::unique_ptr<AbstractMovementController> getMovementController() {
#ifdef _WIN32
	return std::make_unique<DummyMovementController>();
#else
	if (getuid()) {
		spdlog::info("Since this program is not run as root, the DeviceMovementController will not be used. The DummyMovementController will be used instead.");
		return std::make_unique<DummyMovementController>();
	}
	if (gpioInitialise() < 0) {
		spdlog::info("Unable to initialise gpio, using DummyMovementController instead.");
		return std::make_unique<DummyMovementController>();
	}
	return std::make_unique<DeviceMovementController>();
#endif
}