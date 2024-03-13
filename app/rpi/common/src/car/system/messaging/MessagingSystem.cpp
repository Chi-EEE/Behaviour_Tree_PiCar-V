#include "car/system/messaging/MessagingSystem.h"

#include <functional>
#include <memory>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include <nod/nod.hpp>

#include <spdlog/spdlog.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <fmt/format.h>

#include "car/configuration/Configuration.h"

namespace car::system::messaging
{
	MessagingSystem::MessagingSystem()
	{
	}

	void MessagingSystem::initialize(std::shared_ptr<configuration::Configuration> configuration)
	{
		ix::initNetSystem();
		this->setConfiguration(configuration);
		this->handle_message_signal.connect([this](const std::string message)
			{ this->handleMessage(message); });
	}

	void MessagingSystem::initializeWebSocket()
	{
		this->websocket = std::make_unique<ix::WebSocket>();
		this->websocket->disableAutomaticReconnection();

		this->websocket->setUrl("ws://" + this->configuration->host);
		ix::WebSocketHttpHeaders headers;
		headers["code"] = this->configuration->code;
		this->websocket->setExtraHeaders(headers);

		this->websocket->setOnMessageCallback(
			std::bind(&MessagingSystem::onMessageCallback, this, std::placeholders::_1));
	}

	tl::expected<nullptr_t, std::string> MessagingSystem::start()
	{
		initializeWebSocket();

		auto websocket_init_result = this->websocket->connect(1);

		if (!websocket_init_result.success)
		{
			this->websocket->stop();
			this->websocket->close();
			const std::string error_message = "Could not connect to websocket, please check the status of the websocket server.";
			spdlog::error(error_message);
			return tl::make_unexpected(error_message);
		}

		return nullptr;
	}

	void MessagingSystem::stop()
	{
		assert(this->websocket != nullptr);
		this->websocket->stop();
		this->websocket = nullptr;
	}

	void MessagingSystem::terminate()
	{
		this->stop();
		ix::uninitNetSystem();
	}

	void MessagingSystem::setConfiguration(std::shared_ptr<configuration::Configuration> configuration)
	{
		this->configuration = configuration;
	}

	void MessagingSystem::onMessageCallback(const ix::WebSocketMessagePtr& msg) const
	{
		const ix::WebSocketMessage message = *msg;
		this->on_websocket_message_signal(message);
		switch (msg->type)
		{
		case ix::WebSocketMessageType::Open:
		{
			spdlog::info("Connected to the Websocket Server.");
			this->on_connect_signal();
			break;
		}
		case ix::WebSocketMessageType::Close:
		{
			spdlog::info("Disconnected from the Websocket Server; Reason: {}", msg->closeInfo.reason);
			this->on_disconnect_signal(msg->closeInfo.reason);
			break;
		}
		case ix::WebSocketMessageType::Message:
		{
			this->handle_message_signal(msg->str);
			break;
		}
		default:
			break;
		}
	}

	void MessagingSystem::handleMessage(const std::string& message) const
	{
		rapidjson::Document message_json;
		message_json.Parse(message.c_str());
		if (message_json.HasParseError() || !message_json.IsObject())
		{
			spdlog::error("An error has occurred while handling the message: {}", message);
			return;
		}
		if (!message_json.HasMember("type") || !message_json["type"].IsString())
		{
			spdlog::error("Type does not exist in json", message);
			return;
		}
		const std::string type = message_json["type"].GetString();
		if (type == "car")
		{
			return;
		}

		try
		{
			switch (hash(type))
			{
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
		catch (std::exception e)
		{
			spdlog::error("An error has occurred while handling the message: {} | {}", message, e.what());
		}
	}

	void MessagingSystem::handleCommand(const rapidjson::Value& message_json) const
	{
		if (!message_json.HasMember("command") || !message_json["command"].IsString())
		{
			spdlog::error("Command not found or not a string in the JSON.");
			return;
		}

		const std::string command = message_json["command"].GetString();

		switch (hash(command))
		{
		case hash("turn"):
		{
			if (message_json.HasMember("angle") && message_json["angle"].IsFloat())
			{
				float angle = message_json["angle"].GetFloat();
				this->angle_command_signal(angle);
				spdlog::info("Turning by {} angle", angle);
			}
			else
			{
				spdlog::error("Invalid or missing 'angle' in the JSON for 'turn' command.");
			}
			break;
		}
		case hash("move"):
		{
			if (message_json.HasMember("speed") && message_json["speed"].IsInt())
			{
				int speed = message_json["speed"].GetInt();
				speed_command_signal(speed);
				spdlog::info("Moving with {} speed", speed);
			}
			else
			{
				spdlog::error("Invalid or missing 'speed' in the JSON for 'move' command.");
			}
			break;
		}
		case hash("custom"):
		{
			if (message_json.HasMember("custom_type") && message_json.HasMember("custom") &&
				message_json["custom_type"].IsString() && message_json["custom"].IsString())
			{
				const std::string custom_type = message_json["custom_type"].GetString();
				const std::string custom = message_json["custom"].GetString();
				this->custom_command_signal(custom_type, custom);
			}
			else
			{
				spdlog::error("Invalid or missing 'custom_type' or 'custom' in the JSON for 'custom' command.");
			}
			break;
		}
		default:
			spdlog::error("Unknown command: {}", command);
		}
	}

	void MessagingSystem::sendMessage(const std::string& message)
	{
		if (this->websocket != nullptr)
			this->websocket->send(message);
	}
};
