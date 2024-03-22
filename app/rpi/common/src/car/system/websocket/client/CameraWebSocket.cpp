#include "car/system/websocket/client/CameraWebSocket.h"

namespace car::system::websocket::client
{
	void CameraWebSocket::initialize(std::shared_ptr<configuration::Configuration> configuration)
	{
		this->configuration = configuration;
	}

	void CameraWebSocket::restart(std::shared_ptr<configuration::Configuration> configuration)
	{
		this->configuration = configuration;
		this->start();
		this->stop();
	}

	void CameraWebSocket::start()
	{
		this->websocket_ = std::make_unique<ix::WebSocket>();
		this->websocket_->disableAutomaticReconnection();
		this->websocket_->setUrl("ws://" + this->configuration->host);
		ix::WebSocketHttpHeaders headers;
		this->websocket_->setExtraHeaders(headers);
	}

	void CameraWebSocket::stop()
	{
	}

	void CameraWebSocket::terminate()
	{
	}

}
