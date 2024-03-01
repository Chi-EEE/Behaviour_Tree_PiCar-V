#ifndef CARSYSTEM_H
#define CARSYSTEM_H

#pragma once

#include <memory>

#include "car/configuration/Configuration.h"

#include "car/system/lidar/LidarDevice.h"
#include "car/system/messaging/MessagingSystem.h"
#include "car/system/movement/MovementSystem.h"

#include "car/plugin/PluginManager.h"

using namespace car::configuration;
using namespace car::plugin;
using namespace car::system::lidar;
using namespace car::system::messaging;
using namespace car::system::movement;

namespace car::system
{
	class CarSystem : public std::enable_shared_from_this<CarSystem>
	{
	public:
		CarSystem(
			std::shared_ptr<Configuration> configuration,
			std::unique_ptr<LidarDevice> lidar_device,
			std::unique_ptr<MessagingSystem> messaging_system,
			std::unique_ptr<MovementSystem> movement_system,
			std::unique_ptr<PluginManager> plugin_manager);

		void initialize();
		void reload();

		void connectToServer();
		void disconnectFromServer();

		void terminate();

		void update();

		void setConfiguration(std::shared_ptr<Configuration> configuration);

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

		template<typename T>
		const std::shared_ptr<T> getPlugin() const { return this->plugin_manager->getPlugin<T>(); }

		const std::vector<Measure> getScanData() const { return this->lidar_device->getScanData(); }

	private:
		std::shared_ptr<Configuration> configuration;

		const std::unique_ptr<LidarDevice> lidar_device;
		const std::unique_ptr<MessagingSystem> messaging_system;
		const std::unique_ptr<MovementSystem> movement_system;
		const std::unique_ptr<PluginManager> plugin_manager;

		bool connectedToServer = false;
	};
}

#endif