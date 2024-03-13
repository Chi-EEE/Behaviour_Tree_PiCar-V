#ifndef MESSAGINGSYSTEM_H
#define MESSAGINGSYSTEM_H

#pragma once

#include <atomic>
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
		MessagingSystem();

		void initialize(std::shared_ptr<configuration::Configuration> configuration);
		void initializeWebSocket();
		tl::expected<nullptr_t, std::string> start();
		void stop();
		void terminate();

		void setConfiguration(std::shared_ptr<configuration::Configuration> configuration);

		nod::signal<void(const std::string, const std::string)>& getCustomCommandSignal() { return this->custom_command_signal; }
		nod::signal<void(const std::string)>& getHandleMessageSignal() { return this->handle_message_signal; }
		nod::signal<void()>& getConnectSignal() { return this->on_connect_signal; }
		nod::signal<void(const std::string)>& getDisconnectSignal() { return this->on_disconnect_signal; }

		void onMessageCallback(const ix::WebSocketMessagePtr& msg) const;
		std::string getUUID() const { return this->uuid; }
		void handleMessage(const std::string& message) const;
		void handleCommand(const rapidjson::Value& message_json) const;
		void sendMessage(const std::string& message);

		nod::signal<void(const ix::WebSocketMessage msg)> on_websocket_message_signal;

		nod::signal<void()> on_connect_signal;
		nod::signal<void(std::string)> on_disconnect_signal;

		nod::signal<void(const int)> speed_command_signal;
		nod::signal<void(const float)> angle_command_signal;
		nod::signal<void(const std::string)> handle_message_signal;
		nod::signal<void(const std::string, const std::string)> custom_command_signal;

	private:
		tl::expected<std::string, std::string> getFirstMessage();

		std::shared_ptr<configuration::Configuration> configuration;

		std::unique_ptr<ix::WebSocket> websocket;
		std::string websocket_url;

		std::string uuid;
	};
};

#endif