#include "car/system/device/CameraDevice.h"

namespace car::system::device
{
	tl::expected<std::unique_ptr<CameraDevice>, std::string> CameraDevice::create(std::shared_ptr<configuration::Configuration> configuration)
	{
		try {
			return std::make_unique<CameraDevice>(configuration);
		}
		catch (const std::exception& e) {
			return tl::make_unexpected(e.what());
		}
	}

	void CameraDevice::start() {
		std::lock_guard<std::mutex> lock(this->camera_mutex_);
		this->camera_ = std::make_unique<cv::VideoCapture>();
		this->connected_ = this->camera_->open(this->configuration->camera_index);
	}

	void CameraDevice::update() {
		std::lock_guard<std::mutex> lock(this->camera_mutex_);
		if (!this->connected_ || this->camera_ == nullptr || !this->camera_->isOpened()) {
			this->frame_buffer_ = "";
			return;
		}
		const auto now = std::chrono::steady_clock::now();
		if ((now - this->last).count() < this->configuration->getCameraFpsInterval()) {
			this->frame_buffer_ = "";
			return;
		}
		this->last = now;
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
		std::lock_guard<std::mutex> lock(this->camera_mutex_);
		this->camera_->release();
        this->connected_ = false;
	}

	void CameraDevice::disconnect() {
		this->stop();
	}

	void CameraDevice::terminate() {
		this->disconnect();
		this->camera_ = nullptr;
	};
}
