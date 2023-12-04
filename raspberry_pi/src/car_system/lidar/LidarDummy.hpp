#ifndef LIDARDUMMY_HPP
#define LIDARDUMMY_HPP

#pragma once

#include <fstream>

#include <nlohmann/json.hpp>

#include "LidarDevice.hpp"

using json = nlohmann::json;

namespace car_system::lidar {
	class LidarDummy : public LidarDevice
	{
	public:
		LidarDummy()
		{
		};

		void start() const override {};

		std::vector<Measure> scan() const override
		{
			std::vector<Measure> measures;
			return measures;
		};

		void terminate() const override {};

	private:

	};
}

#endif