#include <optional>
#include <string>

#include <fmt/format.h>

#include "global/Config.hpp"

#include "car/display/CarConsole.hpp"

#include "car/system/CarSystem.h"

#include "car/system/lidar/LidarScanner.hpp"
#include "car/system/lidar/LidarDummy.hpp"

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
	using namespace car::display;
	using namespace car::system;
	using namespace car::system::lidar;
	using namespace car::system::messaging;
	using namespace rplidar;

	spdlog::set_level(spdlog::level::off);
	//test();
	
	std::string websocket_url = getWebsocketUrl();
	spdlog::info("Got websocket url: {}", websocket_url);

	std::unique_ptr<LidarDummy> scanner = std::make_unique<LidarDummy>();
	//std::unique_ptr<LidarScanner> scanner = std::make_unique<LidarScanner>("COM3");

	std::unique_ptr<MessagingSystem> messaging_system = std::make_unique<MessagingSystem>(websocket_url);

	auto car_system = std::make_unique<CarSystem>(
		websocket_url,
		std::move(scanner),
		std::move(messaging_system)
	);

	// The CarConsole object will display the UI and handle user input:
	CarConsole car_console(std::move(car_system));
	car_console.run();
	return 0;
}
