#include "car/system/websocket/client/LidarWebSocket.h"

#include <fmt/format.h>

namespace car::system::websocket::client
{
	LidarWebSocket::LidarWebSocket(std::shared_ptr<configuration::Configuration> configuration): configuration_(configuration), websocket_(std::make_unique<ix::WebSocket>())
	{
		this->websocket_->disableAutomaticReconnection();
	}

	LidarWebSocket::~LidarWebSocket()
	{
	}

	void LidarWebSocket::initialize(std::shared_ptr<configuration::Configuration> configuration)
	{
		this->configuration_ = configuration;
	}

	void LidarWebSocket::reload(std::shared_ptr<configuration::Configuration> configuration)
	{
		this->configuration_ = configuration;
	}

	void LidarWebSocket::connect()
	{
		fmt::print("LidarWebSocket::connect()\n");
	}

	void LidarWebSocket::disconnect()
	{
		fmt::print("LidarWebSocket::disconnect()\n");
	}

	void LidarWebSocket::terminate()
	{
		fmt::print("LidarWebSocket::terminate()\n");
	}
}
