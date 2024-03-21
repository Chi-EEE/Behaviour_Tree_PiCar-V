#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H

#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

namespace car::system::camera
{
	class CameraDevice
	{
	public:
		void initialize() const {
			this->camera = std::make_unique<cv::VideoCapture>();
        };
        
		void start() const {
			this->camera->open(0);
        }
		
		std::string getCameraData() const {
			if (!this->camera->isOpened()) {
				return "";
			}
			cv::Mat frame;
			(*this->camera) >> frame;
			if (frame.empty()) {
				return "";
			}
			std::vector<uchar> buf;
			cv::imencode(".jpg", frame, buf);
			return std::string(reinterpret_cast<const char*>(buf.data()), buf.size());
		}

		void stop() const {
			this->camera->release();
		}
		
		void disconnect() const {
			this->stop();
		}
		
		void terminate() const {
			this->disconnect();
			this->camera = nullptr;
		};

	private:
		mutable std::unique_ptr<cv::VideoCapture> camera;
	};
}

#endif