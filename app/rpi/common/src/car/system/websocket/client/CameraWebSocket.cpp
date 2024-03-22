#include "car/system/websocket/client/CameraWebSocket.h"

#include <fmt/format.h>

namespace car::system::websocket::client
{
	CameraWebSocket::CameraWebSocket(std::shared_ptr<configuration::Configuration> configuration) : configuration_(configuration), websocket_(std::make_unique<ix::WebSocket>())
	{
		this->websocket_->disableAutomaticReconnection();
	}

	void CameraWebSocket::reload(std::shared_ptr<configuration::Configuration> configuration)
	{
		this->configuration_ = configuration;
	}

	void CameraWebSocket::restart()
	{
		this->start();
		this->stop();
	}

	void CameraWebSocket::start()
	{
		this->websocket_->setUrl(fmt::format("ws://{host}/camera", fmt::arg("host", this->configuration_->host)));
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
