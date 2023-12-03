#include <iostream>
#include <fstream>
#include <optional>
#include <chrono> // std::chrono::microseconds
#include <thread> // std::this_thread::sleep_for
#include <string>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include <fmt/format.h>

#include <nlohmann/json.hpp>


#include <rplidar/RPLidar.h>

#include "global/Config.hpp"

using json = nlohmann::json;
using namespace rplidar;

std::string get_websocket_url() {
	std::optional<int> maybe_port = GET_CONFIG_VALUE(port);
	if (maybe_port.has_value()) {
		return "ws://" + GET_CONFIG_VALUE(host) + ":" + std::to_string(maybe_port.value()) + "/room/" + GET_CONFIG_VALUE(code);
	}
	return "ws://" + GET_CONFIG_VALUE(host) + "/room/" + GET_CONFIG_VALUE(code);
}

int main()
{
	//spdlog::set_level(spdlog::level::off);

	ix::initNetSystem();

	ix::WebSocket web_socket;

	const std::string websocket_url = get_websocket_url();
	spdlog::info("Connecting to {}", websocket_url);
	web_socket.setUrl(websocket_url);
	web_socket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
		{
			//if (msg->type == ix::WebSocketMessageType::Message)
			//{
			//	std::cout << "received message: " << msg->str << std::endl;
			//	std::cout << "> " << std::flush;
			//}
			//else if (msg->type == ix::WebSocketMessageType::Open)
			//{
			//	std::cout << "Connection established" << std::endl;
			//	std::cout << "> " << std::flush;
			//}
			//else if (msg->type == ix::WebSocketMessageType::Error)
			//{
			//	// Maybe SSL is not configured properly
			//	std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
			//	std::cout << "> " << std::flush;
			//}
			//else if (msg->type == ix::WebSocketMessageType::Close)
			//{
			//	std::cout << "Connection closed: " << msg->closeInfo.code << " " << msg->closeInfo.reason << std::endl;
			//	std::cout << "> " << std::flush;
			//}
		}
	);
	web_socket.start();

	{
		json firstMessage = {{"type", "car"}};
		web_socket.send(firstMessage.dump());
	}

	std::unique_ptr<RPLidar> lidar = RPLidar::create("COM3").value();

	// Stop the lidar scan if it was running
	lidar->reset();
	lidar->stop();
	lidar->stop_motor();

	//auto info_result = lidar->get_info();
	//if (!info_result.has_value()) {
	//	spdlog::error("get_info failed: {}", info_result.error());
	//	return EXIT_FAILURE;
	//}
	//auto info = info_result.value();
	//std::cout << fmt::format("model: {}, firmware: ({}, {}), hardware: {}, serial number: {}\n", info.model, info.firmware.first, info.firmware.second, info.hardware, info.serialNumber);

	//const auto health_result = lidar->get_health();
	//if (!health_result.has_value()) {
	//	spdlog::error("get_health failed: {}", health_result.error());
	//	return EXIT_FAILURE;
	//}
	//const auto health = health_result.value();
	//spdlog::info("({}, {})", health.status, health.errorCode);

	lidar->start_motor();
	const std::function<std::vector<Measure>()> scan_generator = lidar->iter_scans();
	while (true)
	{
		json output = json::array();
		std::vector<Measure> scan = scan_generator();
		for (const Measure& measure : scan)
		{
			output.push_back(
				{
				{"distance", measure.distance},
				{"angle", measure.angle},
				}
			);
		}
		web_socket.send(output.dump());
	}

	lidar->stop();
	lidar->stop_motor();
	lidar->disconnect();
	
    ix::uninitNetSystem();
	
	return 0;
}
