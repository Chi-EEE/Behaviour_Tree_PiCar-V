#include "car/system/device/CameraDevice.h"

namespace car::system::device
{
	tl::expected<std::unique_ptr<CameraDevice>, std::string> CameraDevice::create(std::shared_ptr<configuration::Configuration> configuration)
	{
		try {
			return std::make_unique<CameraDevice>(configuration->camera_index);
		}
		catch (const std::exception& e) {
			return tl::make_unexpected(e.what());
		}
	}

	void CameraDevice::start() {
		std::lock_guard<std::mutex> lock(this->camera_mutex_);
		this->camera_ = std::make_unique<cv::VideoCapture>();
		this->connected_ = this->camera_->open(this->camera_index_);
	}

	void CameraDevice::update() {
		std::lock_guard<std::mutex> lock(this->camera_mutex_);
		if (!this->connected_ || !this->camera_->isOpened()) {
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
		cv::imencode(".avif", frame, buf);
		this->frame_buffer_ = std::string(reinterpret_cast<const char*>(buf.data()), buf.size());
	}

	std::string CameraDevice::getFrameBuffer() const {
		return this->frame_buffer_;
	}

	void CameraDevice::stop() {
		std::lock_guard<std::mutex> lock(this->camera_mutex_);
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
