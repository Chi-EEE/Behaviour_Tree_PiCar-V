#include "car/system/CarSystem.h"

#include <memory>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <tobiaslocker_base64/base64.hpp>

#include "car/configuration/Configuration.h"

#include "car/system/device/DeviceManager.h"
#include "car/system/device/LidarDevice.h"
#include "car/system/device/CameraDevice.h"

#include "car/system/messaging/MessagingSystem.h"
#include "car/system/movement/MovementSystem.h"

#include "car/plugin/PluginManager.h"

namespace car::system
{
	CarSystem::CarSystem(
		std::shared_ptr<Configuration> configuration,
		std::unique_ptr<DeviceManager> device_manager,
		std::unique_ptr<MessagingSystem> messaging_system,
		std::unique_ptr<MovementSystem> movement_system,
		std::unique_ptr<PluginManager> plugin_manager) : configuration_(configuration),
		device_manager_(std::move(device_manager)),
		messaging_system_(std::move(messaging_system)),
		movement_system_(std::move(movement_system)),
		plugin_manager_(std::move(plugin_manager))
	{
	}

	void CarSystem::initialize()
	{
		assert(!this->initialized && "Car System is already initialized.");
		this->messaging_system_->initialize(this->configuration_);
		this->device_manager_->initialize();
		this->movement_system_->initialize();
		this->plugin_manager_->initialize(shared_from_this());
		this->initialized = true;
	}

	void CarSystem::reload()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		this->messaging_system_->setConfiguration(this->configuration_);
	}

	void CarSystem::start()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		// this->lidar_device->start(); // Starting LidarDevice in connect instead.
		this->movement_system_->start();
		this->started = true;
	}

	void CarSystem::stop()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		this->device_manager_->stop();
		this->movement_system_->stop();
		this->plugin_manager_->stop();
	}

	tl::expected<nullptr_t, std::string> CarSystem::tryConnect()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		assert(!this->messaging_system_->isConnected() && "Car System is already connected to the WS Server.");
		auto messaging_system_result = this->messaging_system_->tryConnect();
		if (!messaging_system_result.has_value())
		{
			return tl::make_unexpected(messaging_system_result.error());
		}
		this->device_manager_->start();
		return nullptr;
	}

	void CarSystem::disconnect()
	{
		assert(this->initialized && "Car System has not been initialized yet.");
		assert(this->started && "Car System has not been started yet.");
		//assert(this->messaging_system->isConnected() && "Car System is not connected to the WS Server."); The connect bool is set to false when it disconnects from the websocket
		this->messaging_system_->stop();
		this->device_manager_->stop();
	}

	/// <summary>
	/// Only devices should be terminated here since deconstructor does not work when the program is terminated by the user.
	/// </summary>
	void CarSystem::terminate()
	{
		this->messaging_system_->terminate();
		this->device_manager_->terminate();
		this->movement_system_->terminate();
		this->plugin_manager_->terminate();
	}

	void CarSystem::update()
	{
		this->device_manager_->update();
		if (this->messaging_system_->isConnected())
		{
			rapidjson::Document output_json;
			output_json.SetObject();

			std::string frame_buffer_base64 = base64::to_base64(this->device_manager_->getCameraDevice()->getFrameBuffer());
			auto scan_data = this->device_manager_->getLidarDevice()->getScanData();

			rapidjson::Value data_array(rapidjson::kArrayType);

			for (const Measure& measure : scan_data)
			{
				rapidjson::Value measure_object(rapidjson::kObjectType);
				measure_object.AddMember("distance", measure.distance, output_json.GetAllocator());
				measure_object.AddMember("angle", measure.angle, output_json.GetAllocator());
				data_array.PushBack(measure_object, output_json.GetAllocator());
			}

			output_json.AddMember("lidar", data_array, output_json.GetAllocator());

			output_json.AddMember("frame_buffer_base64", frame_buffer_base64, output_json.GetAllocator());

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			output_json.Accept(writer);

			this->messaging_system_->sendMessage(buffer.GetString());
		}
		this->plugin_manager_->update();
	}

	void CarSystem::setConfiguration(std::shared_ptr<Configuration> configuration)
	{
		this->configuration_ = configuration;
	}
}