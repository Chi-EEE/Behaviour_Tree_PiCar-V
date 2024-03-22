#include "car/system/device/camera/CameraDevice.h"

namespace car::system::device::camera
{
	void CameraDevice::initialize() {
		this->camera_ = std::make_unique<cv::VideoCapture>();
	};

	void CameraDevice::start() {
		this->connected_ = this->camera_->open(0);
	}

	void CameraDevice::update() {
		if (!this->camera_->isOpened()) {
			this->frame_buffer_ = "";
			return;
		}
		cv::Mat frame;
		(*this->camera_) >> frame;
		if (frame.empty()) {
			this->frame_buffer_ = "";
			return;
		}
		std::vector<uchar> buf;
		cv::imencode(".jpg", frame, buf);
		this->frame_buffer_ = std::string(reinterpret_cast<const char*>(buf.data()), buf.size());
	}

	std::string CameraDevice::getFrameBuffer() const {
		return this->frame_buffer_;
	}

	void CameraDevice::stop() {
		this->camera_->release();
	}

	void CameraDevice::disconnect() {
		this->stop();
	}

	void CameraDevice::terminate() {
		this->disconnect();
		this->camera_ = nullptr;
	};
}
