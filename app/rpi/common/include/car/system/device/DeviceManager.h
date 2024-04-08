#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#pragma once

#include <memory>

#include <tl/expected.hpp>

#include "car/configuration/Configuration.h"

#include "CameraDevice.h"
#include "lidar/LidarDevice.h"
#include "lidar/LidarScanner.h"

using namespace car::configuration;

namespace car::system::device
{
	class DeviceManager {
	public:
		[[nodiscard]] static tl::expected<std::unique_ptr<DeviceManager>, std::string> create(std::shared_ptr<Configuration> configuration);

		DeviceManager(std::unique_ptr<CameraDevice> camera_device, std::unique_ptr<lidar::LidarDevice> lidar_device) :
			camera_device_(std::move(camera_device)),
			lidar_device_(std::move(lidar_device))
		{
		}

		CameraDevice* getCameraDevice() {
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
		bool is_initialized_ = false;

		std::unique_ptr<lidar::LidarDevice> lidar_device_;
		std::unique_ptr<CameraDevice> camera_device_;
	};
}

#endif