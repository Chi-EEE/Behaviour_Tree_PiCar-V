#ifndef MESSAGINGSYSTEM_H
#define MESSAGINGSYSTEM_H

#pragma once

#include <functional>
#include <memory>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include <nod/nod.hpp>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include "car/configuration/Configuration.h"

namespace car::system::messaging
{
	// https://stackoverflow.com/a/46711735
	static constexpr uint32_t hash(const std::string_view s) noexcept
	{
		uint32_t hash = 5381;

		for (const char* c = s.data(); c < s.data() + s.size(); ++c)
			hash = ((hash << 5) + hash) + (unsigned char)*c;

		return hash;
	}

	class MessagingSystem
	{
	public:
		MessagingSystem(std::shared_ptr<configuration::Configuration> configuration);

		void initialize();
		void initializeWebSocket();
		void start();
		void stop();
		void terminate();

		nod::signal<void(const std::string, const std::string)>& getCustomCommandSignal() { return this->custom_command_signal; }
		nod::signal<void(const std::string)>& getHandleMessageSignal() { return this->handle_message_signal; }

		void onMessageCallback(const ix::WebSocketMessagePtr& msg) const;
		void handleMessage(const std::string& message) const;
		void handleCommand(const rapidjson::Value& message_json) const;
		void sendMessage(const std::string& message);

		nod::signal<void()> on_websocket_connect_signal;
		nod::signal<void()> on_websocket_disconnect_signal;

		nod::signal<void(const int)> speed_command_signal;
		nod::signal<void(const float)> angle_command_signal;
		nod::signal<void(const std::string)> handle_message_signal;
		nod::signal<void(const std::string, const std::string)> custom_command_signal;

	private:
		std::string getWebSocketUrl();

		std::shared_ptr<configuration::Configuration> configuration;

		std::unique_ptr<ix::WebSocket> websocket;
		std::string websocket_url;
	};
};

#endif