#ifndef LIDARSCANNER_H
#define LIDARSCANNER_H

#pragma once

#include "LidarDevice.h"

#include <memory>
#include <variant>

#include <RPLidar.h>
#include <tl/expected.hpp>

#include "car/configuration/Configuration.h"

using namespace rplidar;

namespace car::system::device::lidar
{
	class LidarScanner final : public LidarDevice
	{
	public:
		[[nodiscard]] static tl::expected<std::unique_ptr<LidarScanner>, std::string> create(std::shared_ptr<configuration::Configuration> configuration) noexcept
		{
			auto maybe_lidar = RPLidar::create(configuration->lidar_port);
			if (maybe_lidar.has_value())
			{
				return std::make_unique<LidarScanner>(configuration, std::move(maybe_lidar.value()));
			}
			else
			{
				return tl::make_unexpected(maybe_lidar.error());
			}
		}

		// Do not call this constructor directly. Use the create method instead.
		LidarScanner(std::shared_ptr<configuration::Configuration> configuration, std::unique_ptr<RPLidar> lidar) : configuration_(configuration), lidar_(std::move(lidar)) {
		}

		void start() final override
		{
			this->lidar_->start_motor();
			this->scan_generator_ = this->lidar_->iter_scans();
		};

		void update() final override
		{
			assert(std::holds_alternative<std::function<std::vector<Measure>()>>(this->scan_generator_) && "LidarScanner::scan() called before start()");
			const auto& scan_generator = std::get<std::function<std::vector<Measure>()>>(this->scan_generator_);
			this->setScanData(scan_generator());
		};

		void stop() final override
		{
			this->scan_generator_ = nullptr;
			this->lidar_->stop();
			this->lidar_->stop_motor();
		}

		void initialize() final override
		{
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
		std::shared_ptr<configuration::Configuration> configuration_;

		std::vector<Measure> scan_data_;

		std::unique_ptr<RPLidar> lidar_;
		std::variant<std::function<std::vector<Measure>()>, nullptr_t> scan_generator_ = nullptr;
	};
}

#endif