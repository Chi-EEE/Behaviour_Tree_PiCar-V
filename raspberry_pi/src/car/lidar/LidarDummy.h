#ifndef LIDARDUMMY_H
#define LIDARDUMMY_H

#pragma once

#include <fstream>

#include <nlohmann/json.hpp>

#include "LidarDevice.h"

using json = nlohmann::json;

namespace car::lidar {
	class LidarDummy : public LidarDevice
	{
	public:
		LidarDummy();

		std::vector<Measure> scan() const override;

	private:

	};
}

#endif