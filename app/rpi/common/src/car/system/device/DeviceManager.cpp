#include "car/system/device/DeviceManager.h"
#include "car/system/CarSystem.h"

namespace car::system::device {
	tl::expected<std::unique_ptr<DeviceManager>, std::string> DeviceManager::create(std::shared_ptr<Configuration> configuration)
	{
		std::unique_ptr<CameraDevice> camera_device = nullptr;
		std::unique_ptr<lidar::LidarDevice> lidar_device = nullptr;
		if (configuration->use_camera) {
			auto maybe_camera_device = CameraDevice::create(configuration);
			if (!maybe_camera_device)
			{
				return tl::make_unexpected(maybe_camera_device.error());
			}
			camera_device = std::move(maybe_camera_device.value());
		}
		if (configuration->use_lidar) {
			auto maybe_lidar_device = lidar::LidarScanner::create(configuration);
			if (!maybe_lidar_device)
			{
				return tl::make_unexpected(maybe_lidar_device.error());
			}
			lidar_device = std::move(maybe_lidar_device.value());
		}
		return std::make_unique<DeviceManager>(std::move(camera_device), std::move(lidar_device));
	}

	void DeviceManager::initialize(std::shared_ptr<system::CarSystem> car_system)
	{
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(!this->is_initialized_ && "The DeviceManager is already initialized");
		if (this->is_initialized_)
		{
			return;
		}
		this->car_system = car_system;
		this->lidar_device_->initialize();
		//this->camera_device_->initialize();
		this->car_system->getMessagingSystem()->getSelectionSignal().connect([&](const std::string message, const rapidjson::Document& message_json) {
			const bool selected = message_json["selected"].GetBool();
			if (selected) {
				this->start();
			}
			else {
				this->stop();
			}
			}
		);
		this->car_system->getMessagingSystem()->getDisconnectSignal().connect([&](const std::string message) {
			this->stop();
			}
		);
		this->is_initialized_ = true;
	}

	void DeviceManager::start()
	{
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(this->is_initialized_ && "The DeviceManager is not initialized");
		this->lidar_device_->start();
		this->camera_device_->start();
		this->is_running_ = true;
	}

	void DeviceManager::update() {
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(this->is_initialized_ && "The DeviceManager is not initialized");
		if (!this->is_initialized_ || !this->is_running_)
		{
			return;
		}

		std::thread lidar_thread([&]() {
			this->lidar_device_->update();
			});

		std::thread camera_thread([&]() {
			this->camera_device_->update();
			});

		lidar_thread.join();
		camera_thread.join();
	}

	void DeviceManager::stop()
	{
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(this->is_initialized_ && "The DeviceManager is not initialized");
		if (!this->is_initialized_)
		{
			return;
		}
		this->lidar_device_->stop();
		this->camera_device_->stop();
		this->is_running_ = false;
	}

	void DeviceManager::terminate()
	{
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(this->is_initialized_ && "The DeviceManager is not initialized");
		if (!this->is_initialized_)
		{
			return;
		}
		this->lidar_device_->terminate();
		this->camera_device_->terminate();
		this->is_initialized_ = false;
	}
} // namespace car::system::device