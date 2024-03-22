#include "car/system/device/DeviceManager.h"

namespace car::system::device {
	void DeviceManager::initialize()
	{
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(!this->initialized_ && "The DeviceManager is already initialized");
		if (this->initialized_)
		{
			return;
		}
		this->lidar_device_->initialize();
		this->camera_device_->initialize();
	}

	void DeviceManager::start()
	{
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(this->initialized_ && "The DeviceManager is not initialized");
		this->lidar_device_->start();
		this->camera_device_->start();
	}

	void DeviceManager::update() {
		assert(this->lidar_device_ != nullptr);
		assert(this->camera_device_ != nullptr);
		assert(this->initialized_ && "The DeviceManager is not initialized");
		if (!this->initialized_)
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
		assert(this->initialized_ && "The DeviceManager is not initialized");
		if (!this->initialized_)
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
		assert(this->initialized_ && "The DeviceManager is not initialized");
		if (!this->initialized_)
		{
			return;
		}
		this->lidar_device_->terminate();
		this->camera_device_->terminate();
		this->initialized_ = false;
	}
} // namespace car::system::device