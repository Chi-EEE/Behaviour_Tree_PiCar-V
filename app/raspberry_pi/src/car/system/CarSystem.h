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

		nod::signal<void(const std::string, const std::string)>& getCustomCommandSignal() const { return this->messaging_system->getCustomCommandSignal(); }
		nod::signal<void(const std::string)>& getHandleMessageSignal() { return this->messaging_system->getHandleMessageSignal(); }

		void startLidarDevice();
		void stopLidarDevice();

		void setRearWheelsSpeed(const int& speed);

		void setRearLeftWheelSpeed(const int& speed);
		void setRearRightWheelSpeed(const int& speed);

		void setFrontWheelsAngle(const float& angle);
		void setCameraServo1Angle(const float& angle);
		void setCameraServo2Angle(const float& angle);

		void setRearWheelDirectionToForward();
		void setRearWheelDirectionToBackward();

		const std::vector<Measure>& getScanData() const { return this->scan_data; }

	private:
		const std::unique_ptr<LidarDevice> lidar_device;
		const std::unique_ptr<MessagingSystem> messaging_system;
		const std::unique_ptr<MovementSystem> movement_system;

		std::vector<Measure> scan_data;

		bool running = false;
	};
}

#endif