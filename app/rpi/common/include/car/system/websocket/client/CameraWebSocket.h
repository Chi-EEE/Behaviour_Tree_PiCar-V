#ifndef CAMERA_WEBSOCKET_H
#define CAMERA_WEBSOCKET_H

#pragma once

#include <memory>

#include <ixwebsocket/IXWebSocket.h>

#include "car/configuration/Configuration.h"

namespace car::system::websocket::client
{
	class CameraWebSocket
	{
	public:
		CameraWebSocket(std::shared_ptr<configuration::Configuration> configuration);
		~CameraWebSocket();

		void initialize(std::shared_ptr<configuration::Configuration> configuration);
		void reload(std::shared_ptr<configuration::Configuration> configuration);
		void restart();
		void start();
		void stop();
		void terminate();

	private:
		std::shared_ptr<configuration::Configuration> configuration_;

		std::unique_ptr<ix::WebSocket> websocket_;
	};
}

#endif