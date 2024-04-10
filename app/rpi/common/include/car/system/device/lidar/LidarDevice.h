#ifndef LIDARDEVICE_H
#define LIDARDEVICE_H

#pragma once

#include <vector>

#include <rapidjson/document.h>

#include <RPLidar.h>

using namespace rplidar;

namespace car::system::device {
	class DeviceManager;
}

namespace car::system::device::lidar
{
	class LidarDevice
	{
	public:
		std::vector<Measure> getScanData() const { return this->scan_data_; }

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void stop() = 0;

		virtual void initialize() = 0;
		virtual void terminate() = 0;
		virtual void disconnect() = 0;

	protected:
		friend class DeviceManager;

		void setScanData(const std::vector<Measure>& scan_data)
		{
			this->scan_data_ = scan_data;
		}

		std::vector<Measure> scan_data_;

		std::mutex scan_data_mutex_;
	};
}

#endif