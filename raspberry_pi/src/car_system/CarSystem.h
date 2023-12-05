#ifndef CAR_H
#define CAR_H

#pragma once

#include <thread> // std::this_thread::sleep_for
#include <memory>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "lidar/LidarDevice.hpp"
#include "messaging/MessagingSystem.hpp"

using namespace car_system::lidar;
using namespace car_system::messaging;

namespace car_system {
	class CarSystem
	{
	public:
		CarSystem(const std::string& websocket_url, std::unique_ptr<LidarDevice> lidar_device, std::unique_ptr<MessagingSystem> messaging_system);
		~CarSystem();

		void initalize();

		void run();
		void terminate();

		void turn(float angle);
		void move(float distance);

	private:
		std::unique_ptr<LidarDevice> lidar_device;
		std::unique_ptr<MessagingSystem> messaging_system;
	};
}

#endif