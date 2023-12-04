#include <iostream>
#include <fstream>
#include <optional>
#include <chrono> // std::chrono::microseconds
#include <thread> // std::this_thread::sleep_for
#include <string>
#include <cstdlib>

#include <fmt/format.h>

#include <nlohmann/json.hpp>

#include <rplidar/RPLidar.h>

#include "global/Config.hpp"

#include "car/Car.h"
#include "car/lidar/LidarScanner.hpp"
#include "car/lidar/LidarDummy.hpp"

using json = nlohmann::json;

using namespace car;
using namespace car::lidar;

using namespace rplidar;

std::string get_websocket_url()
{
	std::optional<int> maybe_port = GET_CONFIG_VALUE(port);
	if (maybe_port.has_value())
	{
		return "ws://" + GET_CONFIG_VALUE(host) + ":" + std::to_string(maybe_port.value()) + "/room/" + GET_CONFIG_VALUE(code);
	}
	return "ws://" + GET_CONFIG_VALUE(host) + "/room/" + GET_CONFIG_VALUE(code);
}

std::unique_ptr<Car> car_vehicle;

void terminate() {
	car_vehicle->terminate();
	spdlog::info("Terminated");
	system("pause");
}

int main()
{
	// spdlog::set_level(spdlog::level::off);
	std::string websocket_url = get_websocket_url();
	spdlog::info("Connecting to {}", websocket_url);

	std::unique_ptr<LidarDummy> dummy_scanner = std::make_unique<LidarDummy>();
	// std::unique_ptr<LidarScanner> scanner = std::make_unique<LidarScanner>("COM3");

	car_vehicle = std::make_unique<Car>(websocket_url, std::move(dummy_scanner));
	std::atexit(terminate);
	car_vehicle->run();

	return 0;
}
