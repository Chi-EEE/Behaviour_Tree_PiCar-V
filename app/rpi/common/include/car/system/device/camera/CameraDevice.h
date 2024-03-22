#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H

#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

namespace car::system::device {
	class DeviceManager;
}

namespace car::system::device::camera
{
	class CameraDevice
	{
	public:
		std::string getFrameBuffer() const;

	protected:
		void initialize();
		void start();
		void update();
		void stop();
		void disconnect();
		void terminate();

		friend class DeviceManager;

	private:
		std::unique_ptr<cv::VideoCapture> camera_;

		bool connected_ = false;
		std::string frame_buffer_;
	};
}

#endif