#ifndef LIDARSCANNER_H
#define LIDARSCANNER_H

#pragma once

#include "LidarDevice.h"

#include <memory>
#include <rplidar/RPLidar.h>

using namespace rplidar;

namespace car::lidar {
	class LidarScanner : public LidarDevice
	{
	public:
		LidarScanner(const std::string& lidar_port);
		~LidarScanner();

		void start() const;
		std::vector<Measure> scan() const override;

	private:
		void initialize() const;

		std::unique_ptr<RPLidar> lidar;
	};
}

#endif