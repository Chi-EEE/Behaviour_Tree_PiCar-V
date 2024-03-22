#ifndef WEBSOCKETMANAGER_H
#define WEBSOCKETMANAGER_H

#pragma once

#include <memory>

#include <ixwebsocket/IXNetSystem.h>

#include "car/configuration/Configuration.h"

#include "client/CameraWebSocket.h"
#include "client/LidarWebSocket.h"

#include "client/GeneralWebSocket.h"

namespace car::system::websocket
{
	class WebSocketManager
	{
	public:
		WebSocketManager();
		~WebSocketManager();

		void initialize(std::shared_ptr<configuration::Configuration> configuration);

		void reload(std::shared_ptr<configuration::Configuration> configuration);

		void connect();
		void disconnect();
		void terminate();

	private:
		bool is_initialized_ = false;

		std::shared_ptr<configuration::Configuration> configuration_;

		std::unique_ptr<client::CameraWebSocket> camera_websocket_;
		std::unique_ptr<client::LidarWebSocket> lidar_websocket_;
		std::unique_ptr<client::GeneralWebSocket> general_websocket_;
	};
}

#endif