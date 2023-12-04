#ifndef CAR_H
#define CAR_H

#pragma once

#include <thread> // std::this_thread::sleep_for
#include <memory>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "lidar/LidarDevice.hpp"

using namespace car_system::lidar;

namespace car_system {
	class CarSystem
	{
	public:
		CarSystem(const std::string& websocket_url, std::unique_ptr<LidarDevice> lidar_device);
		~CarSystem();

		void run();
		void terminate();

		void turn(float angle);
		void move(float distance);

	private:
		void initalize(const std::string& websocket_url);

		std::unique_ptr<LidarDevice> lidar_device;
		ix::WebSocket web_socket;
	};
}

#endif