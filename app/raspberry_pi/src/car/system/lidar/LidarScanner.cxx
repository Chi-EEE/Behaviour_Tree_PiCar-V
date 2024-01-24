#ifndef LIDARSCANNER_CXX
#define LIDARSCANNER_CXX

#pragma once

#include "LidarDevice.cxx"

#include <memory>

#include <RPLidar.h>
#include <spdlog/spdlog.h>
#include <tl/expected.hpp>

using namespace rplidar;

namespace car::system::lidar
{
	class LidarScanner final : public LidarDevice
	{
	public:
		[[nodiscard]]
		static tl::expected<std::unique_ptr<LidarScanner>, nullptr_t> create(const std::string &lidar_port)
		{
			auto maybe_lidar = RPLidar::create(lidar_port);
			if (maybe_lidar.has_value())
			{
				return std::make_unique<LidarScanner>(std::move(maybe_lidar.value()));
			}
			else
			{
				return tl::make_unexpected(nullptr);
			}
		}
		
		LidarScanner(std::unique_ptr<RPLidar> lidar) : lidar(std::move(lidar)){};

		~LidarScanner(){};

		void initialize() const final override
		{
			this->lidar->reset();
			this->lidar->stop();
			this->lidar->stop_motor();
		};

		void start() const final override
		{
			this->lidar->start_motor();
		};

		std::vector<Measure> scan() const final override
		{
			std::function<std::vector<Measure>()> scanGenerator = this->lidar->iter_scans();
			return scanGenerator();
		};

		void stop() const final override
		{
			this->lidar->stop();
			this->lidar->stop_motor();
		}

		void disconnect() const final override
		{
			this->lidar->disconnect();
		}

	private:
		std::unique_ptr<RPLidar> lidar;
	};
}

#endif