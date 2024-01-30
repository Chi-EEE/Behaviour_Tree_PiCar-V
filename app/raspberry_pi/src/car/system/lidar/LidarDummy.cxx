#ifndef LIDARDUMMY_CXX
#define LIDARDUMMY_CXX

#pragma once

#include <fstream>

#include <spdlog/spdlog.h>

#include "LidarDevice.cxx"

namespace car::system::lidar
{
	class LidarDummy final : public LidarDevice
	{
	public:
		LidarDummy()
		{
			spdlog::warn("Currently using the LidarDummy");
		};

		void initialize() const final override{};

		void start() const final override{};

		std::vector<tl::expected<Measure, std::string>> scan() const final override
		{
			std::vector<tl::expected<Measure, std::string>> measures;
			return measures;
		};

		void stop() const final override{};

		void terminate() const final override{};

		void disconnect() const final override{};

	private:
	};
}

#endif