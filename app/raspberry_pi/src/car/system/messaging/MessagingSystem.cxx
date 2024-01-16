#ifndef MESSAGINGSYSTEM_CXX
#define MESSAGINGSYSTEM_CXX

#pragma once

#include <functional>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include <nod/nod.hpp>

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
			try {
				const json message_json = json::parse(message);
				const std::string type = message_json.at("type").get<std::string>();
				switch (hash(type)) {
				case hash("command"):
					this->handleCommand(message_json);
					break;
				case hash("status"):
					spdlog::info("Received status message");
					break;
				default:
					spdlog::info("Received unknown message");
					break;
				}
			}
			catch (std::exception e) {
				spdlog::error("An error has occurred while handling the message: {} | {}", message, e.what());
			}
		}

		void handleCommand(const json& message_json) const {
			switch (hash(message_json.at("command").get<std::string>())) {
			case hash("turn"): {
				float angle = message_json.at("angle").get<float>();
				this->angle_command_signal(angle);
				spdlog::info("Turning by {} angle", angle);
				break;
			}
			case hash("move"): {
				int speed = message_json.at("speed").get<int>();
				this->speed_command_signal(speed);
				spdlog::info("Moving with {} speed", speed);
				break;
			}
			case hash("custom"): {
				const std::string custom_type = message_json.at("custom_type").get<std::string>();
				const std::string custom = message_json.at("custom").get<std::string>();
				this->custom_command_signal(custom_type, custom);
				break;
			}
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