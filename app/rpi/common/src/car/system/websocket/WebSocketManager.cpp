#include "car/system/websocket/WebSocketManager.h"

namespace car::system::websocket
{
	WebSocketManager::WebSocketManager()
	{
		this->camera_websocket_ = std::make_unique<client::CameraWebSocket>();
		this->lidar_websocket_ = std::make_unique<client::LidarWebSocket>();
		this->general_websocket_ = std::make_unique<client::GeneralWebSocket>();
	}

	WebSocketManager::~WebSocketManager()
	{
	}

	void WebSocketManager::initialize(std::shared_ptr<configuration::Configuration> configuration)
	{
		ix::initNetSystem();
		this->configuration_ = configuration;
		this->general_websocket_->initialize(configuration);
		this->camera_websocket_->initialize(configuration);
		this->lidar_websocket_->initialize(configuration);
		this->is_initialized_ = true;
	}

	void WebSocketManager::reload(std::shared_ptr<configuration::Configuration> configuration)
	{
		this->configuration_ = configuration;
		this->general_websocket_->reload(configuration);
		this->camera_websocket_->restart(configuration);
		this->lidar_websocket_->reload(configuration);
	}

	void WebSocketManager::connect()
	{
		auto maybe_success = this->general_websocket_->tryStart();
		if (!maybe_success.has_value()) {
			// TODO:
			return;
		}
		const std::string uuid = this->general_websocket_->getUUID();
		this->camera_websocket_->connect();
		this->lidar_websocket_->connect();
	}

	void WebSocketManager::disconnect()
	{
		this->general_websocket_->disconnect();
		this->camera_websocket_->disconnect();
		this->lidar_websocket_->disconnect();
	}

	void WebSocketManager::terminate()
	{
		this->general_websocket_->terminate();
		this->camera_websocket_->terminate();
		this->lidar_websocket_->terminate();
	}
}