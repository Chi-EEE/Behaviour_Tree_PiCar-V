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
		this->message_signal.connect([this](const std::string message)
									 { this->handleMessage(message); });
		this->on_disconnect_signal.connect([this](const std::string message)
										   { this->onDisconnect(message); });
	}

	void MessagingSystem::initializeWebSocket()
	{
		this->websocket = std::make_unique<ix::WebSocket>(/*ix::WebSocket() // Cannot add this because of Linux*/);
		this->websocket->disableAutomaticReconnection();

		this->websocket->setUrl("ws://" + this->configuration->host);
		ix::WebSocketHttpHeaders headers;
		headers["car_name"] = this->configuration->car_name;
		this->websocket->setExtraHeaders(headers);
	}

	tl::expected<nullptr_t, std::string> MessagingSystem::tryConnect()
	{
		initializeWebSocket();

		auto maybe_uuid = this->getFirstMessage();

		if (!maybe_uuid.has_value())
		{
			return tl::make_unexpected(maybe_uuid.error());
		}

		this->websocket->setOnMessageCallback(
			std::bind(&MessagingSystem::onMessageCallback, this, std::placeholders::_1));

		this->uuid = maybe_uuid.value();

		this->connected = true;

		return nullptr;
	}

	void MessagingSystem::stop()
	{
		if (this->websocket != nullptr)
		{
			this->websocket->stop();
			this->websocket = nullptr;
		}
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

	void MessagingSystem::onMessageCallback(const ix::WebSocketMessagePtr &msg) const
	{
		switch (msg->type)
		{
		case ix::WebSocketMessageType::Close:
		{
			spdlog::info("Disconnected from the Websocket Server; Reason: {}", msg->closeInfo.reason);
			this->on_disconnect_signal(msg->closeInfo.reason);
			break;
		}
		case ix::WebSocketMessageType::Message:
		{
			this->message_signal(msg->str);
			break;
		}
		default:
			break;
		}
	}

	void MessagingSystem::onDisconnect(const std::string message)
	{
		this->connected = false;
	}

	void MessagingSystem::handleMessage(const std::string &message) const
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
		try
		{
			switch (utils::hash(type))
			{
			case utils::hash("command"):
				if (!message_json.HasMember("command") || !message_json["command"].IsString())
				{
					return;
				}
				this->command_signal(message, message_json);
				return;
			default:
				return;
			}
		}
		catch (std::exception e)
		{
			spdlog::error("An error has occurred while handling the message: {} | {}", message, e.what());
		}
	}

	void MessagingSystem::sendMessage(const std::string &message)
	{
		if (this->websocket != nullptr)
			this->websocket->send(message);
	}

	tl::expected<std::string, std::string> MessagingSystem::getFirstMessage()
	{
		std::string error_message;
		std::string uuid;
		std::condition_variable condition;

		this->websocket->setOnMessageCallback([&](const ix::WebSocketMessagePtr &msg)
											  {
				switch (msg->type) {
				case ix::WebSocketMessageType::Close:
				{
					rapidjson::Document error_json;
					error_json.Parse(msg->closeInfo.reason.c_str());
					error_message.append(error_json["message"].GetString());
					condition.notify_one();
					break;
				}
				case ix::WebSocketMessageType::Message:
				{
					rapidjson::Document msg_json;
					msg_json.Parse(msg->str.c_str());
					// Using append instead of assign to avoid copying the string
					uuid.append(msg_json["uuid"].GetString());
					condition.notify_one();
					break;
				}
				case ix::WebSocketMessageType::Error:
				{
					error_message.append(msg->errorInfo.reason.c_str());
					condition.notify_one();
					break;
				}
				} });

		this->websocket->start();

		std::mutex mutex;
		std::unique_lock<std::mutex> lock(mutex);
		condition.wait(lock);

		if (!error_message.empty())
		{
			return tl::make_unexpected(error_message);
		}

		return uuid;
	}
};
