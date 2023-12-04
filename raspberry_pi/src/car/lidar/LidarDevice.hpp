#ifndef LidarDevice_HPP
#define LidarDevice_HPP

#pragma once

#include <vector>

#include <rplidar/RPLidar.h>

using namespace rplidar;

namespace car::lidar {
	class LidarDevice
	{
	public:
		virtual void start() const = 0;
		virtual std::vector<Measure> scan() const = 0;

	private:

	};
} 

#endif