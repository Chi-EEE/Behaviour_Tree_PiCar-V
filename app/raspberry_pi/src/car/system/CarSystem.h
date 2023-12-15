#ifndef CARSYSTEM_H
#define CARSYSTEM_H

#pragma once

#include <memory>

#include <nlohmann/json.hpp>

#include "lidar/LidarDevice.hpp"
#include "messaging/MessagingSystem.hpp"

using json = nlohmann::json;

using namespace car::system::lidar;
using namespace car::system::messaging;

namespace car::system {
	class CarSystem
	{
	public:
		CarSystem(const std::string& websocket_url, std::unique_ptr<LidarDevice> lidar_device, std::unique_ptr<MessagingSystem> messaging_system);
		~CarSystem();

		void initalize();
		void start();

		void update();
		void terminate();

		void move(const MoveCommand& move_command);
		void turn(const TurnCommand& move_command);

	private:
		std::unique_ptr<LidarDevice> lidar_device;
		std::unique_ptr<MessagingSystem> messaging_system;

		bool terminated = false;
	};
}

#endif