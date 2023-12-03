#ifndef CAR_H
#define CAR_H

#pragma once

#include <memory>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "lidar/LidarStrategy.h"

class Car
{
public:
	Car(const std::string& websocket_url, std::unique_ptr<LidarStrategy> lidar);
	~Car();

	void run();

private:
	void initalize(const std::string& websocket_url);
	void terminate();

	std::unique_ptr<LidarStrategy> lidar;
	ix::WebSocket web_socket;
};

#endif