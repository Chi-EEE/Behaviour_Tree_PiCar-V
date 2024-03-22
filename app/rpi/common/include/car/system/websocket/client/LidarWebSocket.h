#ifndef LIDAR_WEBSOCKET_H
#define LIDAR_WEBSOCKET_H

#pragma once

#include <memory>

#include "car/configuration/Configuration.h"

namespace car::system::websocket::client
{
	class LidarWebSocket
	{
	public:
		LidarWebSocket();
		~LidarWebSocket();

		void initialize(std::shared_ptr<configuration::Configuration> configuration);
		void reload(std::shared_ptr<configuration::Configuration> configuration);
		void connect();
		void disconnect();
		void terminate();
	};
}

#endif