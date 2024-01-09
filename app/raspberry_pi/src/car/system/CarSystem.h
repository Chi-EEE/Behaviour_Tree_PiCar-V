#ifndef CARSYSTEM_H
#define CARSYSTEM_H

#pragma once

#include <memory>

#include <nlohmann/json.hpp>

#include "lidar/LidarDevice.cxx"
#include "messaging/MessagingSystem.cxx"
#include "movement/MovementSystem.cxx"

using json = nlohmann::json;

using namespace car::system::lidar;
using namespace car::system::messaging;
using namespace car::system::movement;

namespace car::system {
	class CarSystem
	{
	public:
		CarSystem(const std::string& websocket_url, std::unique_ptr<LidarDevice> lidar_device, std::unique_ptr<MessagingSystem> messaging_system, std::unique_ptr<MovementSystem> movement_system);
		~CarSystem();

		void initialize();
		void start();
		void stop();

		void update();

		void startLidarDevice();
		void stopLidarDevice();

		void setRearWheelsSpeed(const SpeedCommand& speed_command);
		void setFrontWheelsAngle(const AngleCommand& angle_command);

		void setRearLeftWheelSpeed(const SpeedCommand& speed_command);
		void setRearRightWheelSpeed(const SpeedCommand& speed_command);

		void setFrontLeftWheelAngle(const AngleCommand& angle_command);
		void setFrontRightWheelAngle(const AngleCommand& angle_command);

		const std::vector<Measure>& getScanData() const { return this->scan_data; }

	private:
		std::unique_ptr<LidarDevice> lidar_device;
		std::unique_ptr<MessagingSystem> messaging_system;
		std::unique_ptr<MovementSystem> movement_system;

		std::vector<Measure> scan_data;

		bool running = false;
	};
}

#endif