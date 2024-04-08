#include "car/system/device/DeviceManager.h"

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

	void DeviceManager::initialize()
	{
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(!this->is_initialized_ && "The DeviceManager is already initialized");
		if (this->is_initialized_)
		{
			return;
		}
		this->lidar_device_->initialize();
		//this->camera_device_->initialize();
		this->is_initialized_ = true;
	}

	void DeviceManager::start()
	{
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(this->is_initialized_ && "The DeviceManager is not initialized");
		this->lidar_device_->start();
		this->camera_device_->start();
	}

	void DeviceManager::update() {
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(this->is_initialized_ && "The DeviceManager is not initialized");
		if (!this->is_initialized_)
		{
			return;
		}
		this->lidar_device_->update();
		this->camera_device_->update();
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