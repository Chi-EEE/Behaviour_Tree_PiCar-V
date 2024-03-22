#ifndef LIDARDEVICE_H
#define LIDARDEVICE_H

#pragma once

#include <memory>
#include <variant>
#include <vector>

#include <tl/expected.hpp>

#include <RPLidar.h>

#include "car/configuration/Configuration.h"

using namespace rplidar;

namespace car::system::device
{
	class DeviceManager;
	class LidarDevice
	{
	public:
		[[nodiscard]] static tl::expected<std::unique_ptr<LidarDevice>, std::string> create(std::shared_ptr<configuration::Configuration> configuration) noexcept
		{
			return RPLidar::create(configuration->lidar_port).and_then([&](auto&& lidar) {
				return std::make_unique<LidarDevice>(configuration, std::move(lidar));
				});
		}

		LidarDevice(std::shared_ptr<configuration::Configuration> configuration, std::unique_ptr<RPLidar> lidar) : configuration_(configuration), lidar_(std::move(lidar)) {
		}

		void start()
		{
			this->lidar_->start_motor();
			this->scan_generator_ = this->lidar_->iter_scans();
		};

		void stop()
		{
			this->scan_generator_ = nullptr;
			this->lidar_->stop();
			this->lidar_->stop_motor();
		}

		std::vector<Measure> getScanData() const { return this->scan_data_; }
	
	private:
		void initialize()
		{
		};

		void update()
		{
			assert(std::holds_alternative<std::function<std::vector<Measure>()>>(this->scan_generator_) && "LidarScanner::scan() called before start()");
			const auto& scan_generator = std::get<std::function<std::vector<Measure>()>>(this->scan_generator_);
			this->scan_data_ = scan_generator();
		};

		void disconnect()
		{
			this->lidar_->disconnect();
		}

		void terminate()
		{
			this->stop();
			this->disconnect();
		}

		friend class DeviceManager;

	private:
		std::shared_ptr<configuration::Configuration> configuration_;

		std::vector<Measure> scan_data_;

		std::unique_ptr<RPLidar> lidar_;
		std::variant<std::function<std::vector<Measure>()>, nullptr_t> scan_generator_ = nullptr;
	};
}

#endif