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

#include "car_system/CarSystem.h"
#include "car_system/lidar/LidarScanner.hpp"
#include "car_system/lidar/LidarDummy.hpp"

using json = nlohmann::json;

using namespace car_system;
using namespace car_system::lidar;

using namespace rplidar;

std::string get_websocket_url()
{
	std::optional<int> maybe_port = GET_CONFIG_VALUE(port);
	if (maybe_port.has_value())
	{
		return "ws://" + GET_CONFIG_VALUE(host) + ":" + std::to_string(maybe_port.value()) + "/room?room_name=" + GET_CONFIG_VALUE(code);
	}
	return "ws://" + GET_CONFIG_VALUE(host) + "/room/" + GET_CONFIG_VALUE(code);
}

// Car is a global variable so that car.terminate() can be called on exit
std::unique_ptr<CarSystem> car_system_obj;

void terminate() {
	car_system_obj->terminate();
	spdlog::info("Terminated");
	system("pause");
}

int main()
{
	// spdlog::set_level(spdlog::level::off);
	std::string websocket_url = get_websocket_url();
	spdlog::info("Got websocket url: {}", websocket_url);

	std::unique_ptr<LidarDummy> scanner = std::make_unique<LidarDummy>();
	// std::unique_ptr<LidarScanner> scanner = std::make_unique<LidarScanner>("COM3");

	car_system_obj = std::make_unique<CarSystem>(websocket_url, std::move(scanner));
	std::atexit(terminate);
	car_system_obj->run();

	return 0;
}
