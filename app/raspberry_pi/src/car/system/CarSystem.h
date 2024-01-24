#ifndef CARSYSTEM_H
#define CARSYSTEM_H

#pragma once

#include <memory>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "lidar/LidarDevice.cxx"
#include "messaging/MessagingSystem.cxx"
#include "movement/MovementSystem.cxx"

#include "../plugin/Plugin.cxx"

using namespace car::plugin;

using namespace car::system::lidar;
using namespace car::system::messaging;
using namespace car::system::movement;

namespace car::system {
	class CarSystem : public std::enable_shared_from_this<CarSystem>
	{
	public:
		CarSystem(const std::string& websocket_url, std::unique_ptr<LidarDevice> lidar_device, std::unique_ptr<MessagingSystem> messaging_system, std::unique_ptr<MovementSystem> movement_system);
		~CarSystem();

		void initialize();
		void start();
		void stop();

		void update();

		void addPlugin(const std::shared_ptr<car::plugin::Plugin> plugin_);

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

		void setRearWheelsDirectionToForward();

		void setRearLeftWheelDirectionToForward();
		void setRearRightWheelDirectionToForward();

		void setRearWheelsDirectionToBackward();

		void setRearLeftWheelDirectionToBackward();
		void setRearRightWheelDirectionToBackward();

		const std::vector<Measure>& getScanData() const { return this->scan_data; }

	private:
		std::vector<std::weak_ptr<plugin::Plugin>> plugins;

		const std::unique_ptr<LidarDevice> lidar_device;
		const std::unique_ptr<MessagingSystem> messaging_system;
		const std::unique_ptr<MovementSystem> movement_system;

		std::vector<Measure> scan_data;

		bool running = false;
	};
}

#endif