#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#pragma once

#include "camera/CameraDevice.h"
#include "lidar/LidarDevice.h"

namespace car::system::device
{
	class DeviceManager {
	public:
		DeviceManager(std::unique_ptr<camera::CameraDevice> camera_device, std::unique_ptr<lidar::LidarDevice> lidar_device) :
			camera_device_(std::move(camera_device)),
			lidar_device_(std::move(lidar_device))
		{
		}

		camera::CameraDevice* getCameraDevice() {
			return this->camera_device_.get();
		}

		lidar::LidarDevice* getLidarDevice() {
			return this->lidar_device_.get();
		}

		void initialize();
		void start();
		void update();
		void stop();
		void terminate();

	private:
		bool initialized_ = false;

		std::unique_ptr<lidar::LidarDevice> lidar_device_;
		std::unique_ptr<camera::CameraDevice> camera_device_;
	};
}

#endif