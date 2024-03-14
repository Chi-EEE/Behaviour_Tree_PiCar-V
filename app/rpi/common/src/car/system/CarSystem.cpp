#include "car/system/CarSystem.h"

#include <memory>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "car/configuration/Configuration.h"

#include "car/system/lidar/LidarDevice.h"
#include "car/system/messaging/MessagingSystem.h"
#include "car/system/movement/MovementSystem.h"

#include "car/plugin/PluginManager.h"

namespace car::system
{
	CarSystem::CarSystem(
		std::shared_ptr<Configuration> configuration,
		std::unique_ptr<LidarDevice> lidar_device,
		std::unique_ptr<MessagingSystem> messaging_system,
		std::unique_ptr<MovementSystem> movement_system,
		std::unique_ptr<PluginManager> plugin_manager) : configuration(configuration),
		lidar_device(std::move(lidar_device)),
		messaging_system(std::move(messaging_system)),
		movement_system(std::move(movement_system)),
		plugin_manager(std::move(plugin_manager))
	{
	}

	void CarSystem::initialize()
	{
		assert(!this->initialized && "Car System is already initialized.");
		this->messaging_system->initialize(this->configuration);
		this->lidar_device->initialize();
		this->movement_system->initialize();
		this->plugin_manager->initialize(shared_from_this());

		this->messaging_system->speed_command_signal.connect(
			[this](const int speed)
			{
				this->getMovementSystem()->setRearWheelsSpeed(speed);
			});

		this->messaging_system->angle_command_signal.connect(
			[this](const float angle)
			{
				this->getMovementSystem()->setFrontWheelsAngle(angle);
			});
	
		this->initialized = true;
	}

	void CarSystem::reload()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		this->messaging_system->setConfiguration(this->configuration);
	}

	void CarSystem::start()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		this->lidar_device->start();
		this->movement_system->start();
		this->started = true;
	}

	void CarSystem::stop()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		this->lidar_device->stop();
		this->movement_system->stop();
		this->plugin_manager->stop();
	}

	tl::expected<nullptr_t, std::string> CarSystem::tryConnect()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		assert(!this->connectedToServer && "Car System is already connected to the WS Server.");
		auto messaging_system_result = this->messaging_system->tryConnect();
		if (!messaging_system_result.has_value())
		{
			return tl::make_unexpected(messaging_system_result.error());
		}
		this->connectedToServer = true;
		return nullptr;
	}

	void CarSystem::disconnect()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		assert(this->connectedToServer && "Car System is not connected to the WS Server.");
		this->connectedToServer = false;
		this->messaging_system->stop();
	}

	/// <summary>
	/// Only devices should be terminated here since deconstructor does not work when the program is terminated by the user.
	/// </summary>
	void CarSystem::terminate()
	{
		this->connectedToServer = false;
		this->messaging_system->terminate();
		this->lidar_device->terminate();
		this->movement_system->terminate();
		this->plugin_manager->terminate();
	}

	void CarSystem::update()
	{
		if (this->connectedToServer)
		{
			this->messaging_system->sendMessage(this->lidar_device->getLidarMessage());
		}
		this->plugin_manager->update();
	}

	void CarSystem::setConfiguration(std::shared_ptr<Configuration> configuration)
	{
		this->configuration = configuration;
	}
}