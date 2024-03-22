#ifndef GENERAL_WEBSOCKET_H
#define GENERAL_WEBSOCKET_H

#pragma once

#include <memory>

#include <ixwebsocket/IXWebSocket.h>
#include <tl/expected.hpp>

#include "car/configuration/Configuration.h"

namespace car::system::websocket::client
{
	class GeneralWebSocket
	{
	public:
		GeneralWebSocket();
		~GeneralWebSocket();

		void initialize(std::shared_ptr<configuration::Configuration> configuration);
		[[nodiscard]] tl::expected<nullptr_t, std::string> tryStart();
		void stop();
		void restart(std::shared_ptr<configuration::Configuration> configuration);
		void terminate();

		std::string getUUID() const { return this->uuid_; }

	private:
		std::shared_ptr<configuration::Configuration> configuration_;
		std::unique_ptr<ix::WebSocket> websocket_;

		std::string uuid_;
	};
}

#endif