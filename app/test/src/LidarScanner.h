#ifndef LIDARSCANNER_H
#define LIDARSCANNER_H

#pragma once

#include "LidarDevice.h"

#include <memory>
#include <variant>

#include <RPLidar.h>
#include <tl/expected.hpp>

using namespace rplidar;

namespace car::system::lidar
{
	class LidarScanner final : public LidarDevice
	{
	public:
		[[nodiscard]] static tl::expected<std::unique_ptr<LidarScanner>, nullptr_t> create(const std::string& lidar_port)
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

		LidarScanner(std::unique_ptr<RPLidar> lidar) : lidar(std::move(lidar)) {};

		void initialize() const final override
		{
		};

		void start() const final override
		{
			this->lidar->start_motor();
			this->scan_generator = this->lidar->iter_scans();
		};

		std::vector<Measure> scan() const final override
		{
			assert(std::holds_alternative<std::function<std::vector<Measure>()>>(this->scan_generator) && "LidarScanner::scan() called before start()");
			const auto& scan_generator = std::get<std::function<std::vector<Measure>()>>(this->scan_generator);
			return scan_generator();
		};

		void stop() const final override
		{
			this->scan_generator = nullptr;
			this->lidar->stop();
			this->lidar->stop_motor();
		}

		void disconnect() const final override
		{
			this->lidar->disconnect();
		}

		void terminate() const final override
		{
			this->stop();
			this->disconnect();
		}

	private:
		std::unique_ptr<RPLidar> lidar;
		mutable std::variant<std::function<std::vector<Measure>()>, nullptr_t> scan_generator = nullptr;
	};
}

#endif