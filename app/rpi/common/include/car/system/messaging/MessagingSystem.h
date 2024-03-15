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

#include "utils/Utility.hpp"

#include "car/configuration/Configuration.h"

namespace car::system::messaging
{
	class MessagingSystem
	{
	public:
		MessagingSystem();

		void initialize(std::shared_ptr<configuration::Configuration> configuration);
		void initializeWebSocket();
		tl::expected<nullptr_t, std::string> tryConnect();
		void stop();
		void terminate();

		// Necessary for the reloading the configuration
		void setConfiguration(std::shared_ptr<configuration::Configuration> configuration);

		nod::signal<void(const std::string, const rapidjson::Value&)>& getCommandSignal() { return this->command_signal; }
		nod::signal<void(const std::string)>& getMessageSignal() { return this->message_signal; }
		nod::signal<void(const std::string)>& getDisconnectSignal() { return this->on_disconnect_signal; }

		void onMessageCallback(const ix::WebSocketMessagePtr& msg) const;
		void onDisconnect(const std::string);

		std::string getUUID() const { return this->uuid; }
		void handleMessage(const std::string& message) const;
		void sendMessage(const std::string& message);

		const bool isConnected() const { return this->connected; }

		nod::signal<void(std::string)> on_disconnect_signal;

		nod::signal<void(const std::string)> message_signal;
		nod::signal<void(const std::string, const rapidjson::Value&)> command_signal;

	private:
		tl::expected<std::string, std::string> getFirstMessage();

		std::shared_ptr<configuration::Configuration> configuration;

		std::unique_ptr<ix::WebSocket> websocket;
		std::string websocket_url;

		std::string uuid;

		bool connected = false;
	};
};

#endif
