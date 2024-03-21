#include "car/system/CarSystem.h"

#include <memory>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "car/configuration/Configuration.h"
#include "car/system/lidar/LidarDevice.h"
#include "car/system/camera/CameraDevice.h"
#include "car/system/messaging/MessagingSystem.h"
#include "car/system/movement/MovementSystem.h"

#include "car/plugin/PluginManager.h"

namespace car::system
{
	CarSystem::CarSystem(
		std::shared_ptr<Configuration> configuration,
		std::unique_ptr<LidarDevice> lidar_device,
		std::unique_ptr<CameraDevice> camera_device,
		std::unique_ptr<MessagingSystem> messaging_system,
		std::unique_ptr<MovementSystem> movement_system,
		std::unique_ptr<PluginManager> plugin_manager) : configuration(configuration),
		lidar_device(std::move(lidar_device)),
		camera_device(std::move(camera_device)),
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
		this->camera_device->initialize();
		this->movement_system->initialize();
		this->plugin_manager->initialize(shared_from_this());
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
		// this->lidar_device->start(); // Starting LidarDevice in connect instead.
		this->movement_system->start();
		this->started = true;
	}

	void CarSystem::stop()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		this->lidar_device->stop();
		this->camera_device->stop();
		this->movement_system->stop();
		this->plugin_manager->stop();
	}

	tl::expected<nullptr_t, std::string> CarSystem::tryConnect()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		assert(!this->messaging_system->isConnected() && "Car System is already connected to the WS Server.");
		auto messaging_system_result = this->messaging_system->tryConnect();
		if (!messaging_system_result.has_value())
		{
			return tl::make_unexpected(messaging_system_result.error());
		}
		this->camera_device->start();
		this->lidar_device->start();
		return nullptr;
	}

	void CarSystem::disconnect()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		//assert(this->messaging_system->isConnected() && "Car System is not connected to the WS Server."); The connect bool is set to false when it disconnects from the websocket
		this->messaging_system->stop();
		this->lidar_device->stop();
		this->camera_device->stop();
	}

	/// <summary>
	/// Only devices should be terminated here since deconstructor does not work when the program is terminated by the user.
	/// </summary>
	void CarSystem::terminate()
	{
		this->messaging_system->terminate();
		this->lidar_device->terminate();
		this->camera_device->terminate();
		this->movement_system->terminate();
		this->plugin_manager->terminate();
	}

	void CarSystem::update()
	{
		if (this->messaging_system->isConnected())
		{
			rapidjson::Document output_json;
			output_json.SetObject();

			rapidjson::Document lidar_json = this->lidar_device->getLidarJson();
			output_json.AddMember("lidar_data", lidar_json, lidar_json.GetAllocator());

			std::string camera_data = this->camera_device->getCameraData();

			rapidjson::Value camera_data_json;
			camera_data_json.SetString(camera_data.c_str(), output_json.GetAllocator());
			output_json.AddMember("camera_data", camera_data_json, output_json.GetAllocator());

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			output_json.Accept(writer);
			
			std::string output_string = buffer.GetString();
			this->messaging_system->sendMessage(output_string);
		}
		this->plugin_manager->update();
	}

	void CarSystem::setConfiguration(std::shared_ptr<Configuration> configuration)
	{
		this->configuration = configuration;
	}
}