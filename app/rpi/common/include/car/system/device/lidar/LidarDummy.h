#ifndef LIDARDUMMY_H
#define LIDARDUMMY_H

#pragma once

#include <fstream>
#include <spdlog/spdlog.h>

#include "LidarDevice.h"

namespace car::system::device::lidar
{
	class LidarDummy final : public LidarDevice
	{
	public:
		LidarDummy()
		{
			spdlog::warn("Currently using the LidarDummy");
		};

		void start() final override {};
		void update() final override {};
		void stop() final override {};
		void initialize() final override {};
		void terminate() final override {};
		void disconnect() final override {};

	private:
	};
}

#endif