#ifndef LIDARDUMMY_HPP
#define LIDARDUMMY_HPP

#pragma once

#include <fstream>

#include "LidarDevice.hpp"

namespace car::system::lidar {
	class LidarDummy : public LidarDevice
	{
	public:
		LidarDummy()
		{
		};

		void initialize() const override {};

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