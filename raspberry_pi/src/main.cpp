#include <iostream>
#include <fstream>
#include <optional>
#include <chrono>  std::chrono::microseconds
#include <thread>  std::this_thread::sleep_for
#include <string>
#include <cstdlib>

#include <fmt/format.h>

#include <nlohmann/json.hpp>

#include <rplidar/RPLidar.h>

#include "global/Config.hpp"

#include "car_system/CarSystem.h"
#include "car_system/CarConsole.hpp"

#include "car_system/lidar/LidarScanner.hpp"
#include "car_system/lidar/LidarDummy.hpp"

using json = nlohmann::json;

using namespace car_system;
using namespace car_system::lidar;

using namespace rplidar;

std::string getWebsocketUrl()
{
	std::optional<int> maybe_port = GET_CONFIG_VALUE(port);
	if (maybe_port.has_value())
	{
		return fmt::format("ws://{}:{}/ws/room?request=join&type=car&room_name={}", GET_CONFIG_VALUE(host), maybe_port.value(), GET_CONFIG_VALUE(room));
	}
	return fmt::format("ws://{}/ws/room?request=join&type=&room_name={}", GET_CONFIG_VALUE(host), GET_CONFIG_VALUE(room));
}

int main()
{
	spdlog::set_level(spdlog::level::off);
	//test();
	
	std::string websocket_url = getWebsocketUrl();
	spdlog::info("Got websocket url: {}", websocket_url);

	std::unique_ptr<LidarDummy> scanner = std::make_unique<LidarDummy>();
	//std::unique_ptr<LidarScanner> scanner = std::make_unique<LidarScanner>("COM3");

	std::unique_ptr<MessagingSystem> messaging_system = std::make_unique<MessagingSystem>(websocket_url);

	auto car_system_obj = std::make_unique<CarSystem>(
		websocket_url,
		std::move(scanner),
		std::move(messaging_system)
	);

	// The CarConsole object will display the UI and handle user input:
	CarConsole car_console(std::move(car_system_obj));
	car_console.run();
	return 0;
}
