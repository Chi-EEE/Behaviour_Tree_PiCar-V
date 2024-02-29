#ifndef LIDARDUMMY_H
#define LIDARDUMMY_H

#pragma once

#include <fstream>

#include "LidarDevice.h"

namespace car::system::lidar
{
	class LidarDummy final : public LidarDevice
	{
	public:
		LidarDummy();

		void initialize() const final override{};

		void start() const final override{};

		std::vector<Measure> scan() const final override
		{
			std::vector<Measure> measures;
			return measures;
		};

		void stop() const final override{};

		void terminate() const final override{};

		void disconnect() const final override{};

	private:
	};
}

#endif