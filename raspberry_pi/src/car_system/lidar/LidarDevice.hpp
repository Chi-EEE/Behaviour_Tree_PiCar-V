#ifndef LidarDevice_HPP
#define LidarDevice_HPP

#pragma once

#include <vector>

#include <rplidar/RPLidar.h>

using namespace rplidar;

namespace car_system::lidar {
	class LidarDevice
	{
	public:
		virtual void start() const = 0;
		virtual std::vector<Measure> scan() const = 0;
		virtual void terminate() const = 0;

	private:

	};
} 

#endif