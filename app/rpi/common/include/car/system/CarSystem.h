#ifndef CARSYSTEM_H
#define CARSYSTEM_H

#pragma once

#include <memory>

#include "car/configuration/Configuration.h"

#include "car/system/lidar/LidarDevice.h"
#include "car/system/camera/CameraDevice.h"
#include "car/system/messaging/MessagingSystem.h"
#include "car/system/movement/MovementSystem.h"

#include "car/plugin/PluginManager.h"

using namespace car::configuration;
using namespace car::plugin;
using namespace car::system::lidar;
using namespace car::system::camera;
using namespace car::system::messaging;
using namespace car::system::movement;

namespace car::system
{
	// Make sure this is stored as a shared_ptr
	class CarSystem : public std::enable_shared_from_this<CarSystem>
	{
	public:
		CarSystem(
			std::shared_ptr<Configuration> configuration,
			std::unique_ptr<LidarDevice> lidar_device,
			std::unique_ptr<CameraDevice> camera_device,
			std::unique_ptr<MessagingSystem> messaging_system,
			std::unique_ptr<MovementSystem> movement_system,
			std::unique_ptr<PluginManager> plugin_manager);

		void initialize();
		void reload();

		void start();
		void stop();

		tl::expected<nullptr_t, std::string> tryConnect();
		void disconnect();

		void terminate();

		void update();

		const std::shared_ptr<Configuration> getConfiguration() const { return this->configuration; };
		void setConfiguration(std::shared_ptr<Configuration> configuration);

		LidarDevice *getLidarDevice() const
		{
			return this->lidar_device.get();
		}

		MessagingSystem *getMessagingSystem() const
		{
			return this->messaging_system.get();
		}

		MovementSystem *getMovementSystem() const
		{
			return this->movement_system.get();
		}

		template <typename T>
		const std::shared_ptr<T> getPlugin() const { return this->plugin_manager->getPlugin<T>(); }

		const std::vector<Measure> getScanData() const { return this->lidar_device->getScanData(); }

	private:
		std::shared_ptr<Configuration> configuration;

		const std::unique_ptr<LidarDevice> lidar_device;
		const std::unique_ptr<CameraDevice> camera_device;
		const std::unique_ptr<MessagingSystem> messaging_system;
		const std::unique_ptr<MovementSystem> movement_system;
		const std::unique_ptr<PluginManager> plugin_manager;

		bool initialized = false;
		bool started = false;
	};
}

#endif