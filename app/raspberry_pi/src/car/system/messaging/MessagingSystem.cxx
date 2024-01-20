#ifndef MESSAGINGSYSTEM_CXX
#define MESSAGINGSYSTEM_CXX

#pragma once

#include <functional>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include <nod/nod.hpp>

#include <spdlog/spdlog.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

namespace car::system::messaging {
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
		MessagingSystem(const std::string& websocket_url) : websocket_url(websocket_url) {
		};

		void initialize()
		{
			ix::initNetSystem();
			this->websocket.setUrl(websocket_url);
			this->websocket.setOnMessageCallback(
				std::bind(&MessagingSystem::onMessageCallback, this, std::placeholders::_1)
			);
			this->handle_message_signal.connect([this](const std::string message)
				{
					this->handleMessage(message);
				}
			);
		}

		void start()
		{
			this->websocket.start();
			bool open = false;
			for (int i = 0; i < 3; i++) {
				if (this->websocket.getReadyState() == ix::ReadyState::Open) {
					open = true;
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(i * 3000));
			}
			if (!open) {
				spdlog::error("Could not connect to websocket");
				return;
			}
			else
			{
				spdlog::info("Connected to websocket");
			}
		}

		void stop() {
			this->websocket.stop();
			ix::uninitNetSystem();
		}

		nod::signal<void(const std::string, const std::string)>& getCustomCommandSignal() { return this->custom_command_signal; }
		nod::signal<void(const std::string)>& getHandleMessageSignal() { return this->handle_message_signal; }

		void onMessageCallback(const ix::WebSocketMessagePtr& msg) const
		{
			switch (msg->type) {
			case ix::WebSocketMessageType::Open:
			{
				spdlog::info("WebSocket connected");
				this->on_websocket_connect_signal();
				break;
			}
			case ix::WebSocketMessageType::Close:
			{
				spdlog::info("WebSocket closed");
				this->on_websocket_disconnect_signal();
				break;
			}
			case ix::WebSocketMessageType::Message:
			{
				this->handle_message_signal(msg->str);
			}
			}

		}

		void handleMessage(const std::string& message) const {
			rapidjson::Document message_json;
			message_json.Parse(message.c_str());
			if (message_json.HasParseError() || !message_json.IsObject()) {
				spdlog::error("An error has occurred while handling the message: {}", message);
				return;
			}
			if (!message_json.HasMember("type") || !message_json["type"].IsString()) {
				spdlog::error("Type does not exist in json", message);
				return;
			}
			const std::string type = message_json["type"].GetString();
			if (type == "car") {
				return;
			}

			try {
				switch (hash(type)) {
				case hash("command"):
					this->handleCommand(message_json);
					break;
				case hash("status"):
					spdlog::info("Received status message");
					break;
				default:
					break;
				}
			}
			catch (std::exception e) {
				spdlog::error("An error has occurred while handling the message: {} | {}", message, e.what());
			}
		}

		void handleCommand(const rapidjson::Value& message_json) const {
			if (!message_json.HasMember("command") || !message_json["command"].IsString()) {
				spdlog::error("Command not found or not a string in the JSON.");
				return;
			}

			const std::string command = message_json["command"].GetString();

			switch (hash(command)) {
			case hash("turn"): {
				if (message_json.HasMember("angle") && message_json["angle"].IsFloat()) {
					float angle = message_json["angle"].GetFloat();
					angle_command_signal(angle);
					spdlog::info("Turning by {} angle", angle);
				}
				else {
					spdlog::error("Invalid or missing 'angle' in the JSON for 'turn' command.");
				}
				break;
			}
			case hash("move"): {
				if (message_json.HasMember("speed") && message_json["speed"].IsInt()) {
					int speed = message_json["speed"].GetInt();
					speed_command_signal(speed);
					spdlog::info("Moving with {} speed", speed);
				}
				else {
					spdlog::error("Invalid or missing 'speed' in the JSON for 'move' command.");
				}
				break;
			}
			case hash("custom"): {
				if (message_json.HasMember("custom_type") && message_json.HasMember("custom") &&
					message_json["custom_type"].IsString() && message_json["custom"].IsString()) {
					const std::string custom_type = message_json["custom_type"].GetString();
					const std::string custom = message_json["custom"].GetString();
					custom_command_signal(custom_type, custom);
				}
				else {
					spdlog::error("Invalid or missing 'custom_type' or 'custom' in the JSON for 'custom' command.");
				}
				break;
			}
			default:
				spdlog::error("Unknown command: {}", command);
			}
		}

		void sendMessage(const std::string& message) {
			this->websocket.send(message);
		}

		~MessagingSystem() {
		};

		nod::signal<void()> on_websocket_connect_signal;
		nod::signal<void()> on_websocket_disconnect_signal;

		nod::signal<void(const int)> speed_command_signal;
		nod::signal<void(const float)> angle_command_signal;
		nod::signal<void(const std::string)> handle_message_signal;
		nod::signal<void(const std::string, const std::string)> custom_command_signal;
	private:
		ix::WebSocket websocket;
		std::string websocket_url;
	};
};

#endif