#ifndef MESSAGINGSYSTEM_H
#define MESSAGINGSYSTEM_H

#pragma once

#include <functional>
#include <memory>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>

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
		const tl::expected<nullptr_t, std::string> tryConnect();
		void stop();
		void terminate();

		// Necessary for the reloading the configuration
		void setConfiguration(std::shared_ptr<configuration::Configuration> configuration);

		nod::signal<void(const std::string, const rapidjson::Document&)>& getCommandSignal() { return this->command_signal_; }
		nod::signal<void(const std::string, const rapidjson::Document&)>& getSelectionSignal() { return this->selection_signal_; }
		nod::signal<void(const std::string)>& getMessageSignal() { return this->message_signal_; }
		nod::signal<void(const std::string)>& getDisconnectSignal() { return this->on_disconnect_signal_; }

		void onMessageCallback(const ix::WebSocketMessagePtr& msg) const;
		void onDisconnect(const std::string);

		const std::string getUUID() const { return this->uuid_; }
		void handleMessage(const std::string& message) const;
		void sendMessage(const std::string& message);

		struct FirstMessageStruct
		{
			std::string error_message;
			std::string uuid;
			std::condition_variable condition;
		};
		void onFirstMessage(const ix::WebSocketMessagePtr& msg, FirstMessageStruct& first_message_struct);

		const bool isConnected() const { return this->connected_; }

		nod::signal<void(std::string)> on_disconnect_signal_;

		nod::signal<void(const std::string)> message_signal_;
		nod::signal<void(const std::string, const rapidjson::Document&)> command_signal_;
		nod::signal<void(const std::string, const rapidjson::Document&)> selection_signal_;

	private:
		tl::expected<std::string, std::string> getFirstMessage();

		std::shared_ptr<configuration::Configuration> configuration_;

		std::unique_ptr<ix::WebSocket> websocket_;
		std::string websocket_url_;

		std::string uuid_;

		bool connected_ = false;
	};
};

#endif
