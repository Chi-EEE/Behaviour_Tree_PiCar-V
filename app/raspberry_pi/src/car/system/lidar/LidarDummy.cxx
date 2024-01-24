#ifndef LIDARDUMMY_CXX
#define LIDARDUMMY_CXX

#pragma once

#include <fstream>

#include <spdlog/spdlog.h>

#include "LidarDevice.cxx"

namespace car::system::lidar {
	class LidarDummy : public LidarDevice
	{
	public:
		LidarDummy()
		{
			spdlog::warn("Currently using the LidarDummy");
		};

		void initialize() const final override {};

		void start() const final override {};

		std::vector<Measure> scan() const final override
		{
			std::vector<Measure> measures;
			return measures;
		};

		void stop() const final override {};

		void disconnect() const final override {};
	private:

	};
}

#endif