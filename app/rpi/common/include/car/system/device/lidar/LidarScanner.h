#ifndef LIDARSCANNER_H
#define LIDARSCANNER_H

#pragma once

#include "LidarDevice.h"

#include <memory>
#include <variant>

#include <RPLidar.h>
#include <tl/expected.hpp>

using namespace rplidar;

namespace car::system::device::lidar
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

		// Do not call this constructor directly. Use the create method instead.
		LidarScanner(std::unique_ptr<RPLidar> lidar) : lidar_(std::move(lidar)) {};

		void start() final override
		{
			this->lidar_->start_motor();
			this->scan_generator_ = this->lidar_->iter_scans();
		};

		void stop() final override
		{
			this->scan_generator_ = nullptr;
			this->lidar_->stop();
			this->lidar_->stop_motor();
		}

	private:
		void initialize() final override
		{
		};

		void update() final override
		{
			assert(std::holds_alternative<std::function<std::vector<Measure>()>>(this->scan_generator_) && "LidarScanner::scan() called before start()");
			const auto& scan_generator = std::get<std::function<std::vector<Measure>()>>(this->scan_generator_);
			this->setScanData(scan_generator());
		};

		void disconnect() final override
		{
			this->lidar_->disconnect();
		}

		void terminate() final override
		{
			this->stop();
			this->disconnect();
		}

	private:
		std::unique_ptr<RPLidar> lidar_;
		std::variant<std::function<std::vector<Measure>()>, nullptr_t> scan_generator_ = nullptr;
	};
}

#endif