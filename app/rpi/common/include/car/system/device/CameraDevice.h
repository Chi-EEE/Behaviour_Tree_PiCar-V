#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H

#pragma once

#include <vector>

#include <tl/expected.hpp>
#include <opencv2/opencv.hpp>

#include "car/configuration/Configuration.h"

namespace car::system::device
{
	class DeviceManager;
	class CameraDevice
	{
	public:
		CameraDevice(std::shared_ptr<configuration::Configuration> configuration) : configuration(configuration) {}

		CameraDevice(const CameraDevice&) = delete;
		CameraDevice& operator=(const CameraDevice&) = delete;

		CameraDevice(CameraDevice&&) = delete;
		CameraDevice& operator=(CameraDevice&&) = delete;

		~CameraDevice() = default;

	public:
		[[nodiscard]] static tl::expected<std::unique_ptr<CameraDevice>, std::string> create(std::shared_ptr<configuration::Configuration> configuration);
		std::string getFrameBuffer() const;

	protected:
		void start();
		void update();
		void stop();
		void disconnect();
		void terminate();

		friend class DeviceManager;

	private:
		std::shared_ptr<configuration::Configuration> configuration;

		std::unique_ptr<cv::VideoCapture> camera_;

		bool connected_ = false;
		std::string frame_buffer_;

		std::mutex camera_mutex_;
	
		std::chrono::steady_clock::time_point last;
	};
}

#endif