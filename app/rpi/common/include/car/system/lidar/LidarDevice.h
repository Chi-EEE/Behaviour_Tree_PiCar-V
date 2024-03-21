#ifndef LIDARDEVICE_H
#define LIDARDEVICE_H

#pragma once

#include <vector>

#include <rapidjson/document.h>

#include <RPLidar.h>

using namespace rplidar;

namespace car::system::lidar
{
	class LidarDevice
	{
	public:
		virtual void initialize() const = 0;
		virtual void start() const = 0;

		virtual std::vector<Measure> scan() const = 0;

		rapidjson::Document getLidarJson();

		std::vector<Measure> getScanData() const
		{
			return this->scan_data;
		}

		virtual void stop() const = 0;
		virtual void terminate() const = 0;
		virtual void disconnect() const = 0;

	private:
		std::vector<Measure> scan_data;
	};
}

#endif