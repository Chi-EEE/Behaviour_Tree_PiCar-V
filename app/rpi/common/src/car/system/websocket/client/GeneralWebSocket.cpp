#include "car/system/websocket/client/GeneralWebSocket.h"

#include <fmt/format.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <spdlog/spdlog.h>

namespace car::system::websocket::client
{
	GeneralWebSocket::GeneralWebSocket()
	{

	}

	GeneralWebSocket::~GeneralWebSocket()
	{

	}

	void GeneralWebSocket::initialize(std::shared_ptr<configuration::Configuration> configuration)
	{
		this->configuration_ = configuration;
	}

	tl::expected<nullptr_t, std::string> GeneralWebSocket::tryStart()
	{
		this->websocket_ = std::make_unique<ix::WebSocket>();
		this->websocket_->disableAutomaticReconnection();
		this->websocket_->setUrl(fmt::format("ws://{host}/general", fmt::arg("host", this->configuration_->host)));

		ix::WebSocketHttpHeaders headers;
		this->websocket_->setExtraHeaders(headers);

		std::string error_message;
		std::string uuid;
		std::condition_variable condition;

		auto onFirstMessage = [&](const ix::WebSocketMessagePtr& msg)
			{
				try {
					switch (msg->type)
					{
					case ix::WebSocketMessageType::Open:
					{
						break;
					}
					case ix::WebSocketMessageType::Close:
					{
						const std::string error_closeInfo_reason = msg->closeInfo.reason;
						rapidjson::Document error_json;
						error_json.Parse(error_closeInfo_reason.c_str());
						if (!error_json.HasMember("message") || !error_json["message"].IsString())
						{
							spdlog::error("MessagingSystem::onFirstMessage: No message in json: {}", error_closeInfo_reason);
							condition.notify_one();
							return;
						}
						error_message.append(error_json["message"].GetString());
						condition.notify_one();
						break;
					}
					case ix::WebSocketMessageType::Message:
					{
						const std::string message = msg->str;
						rapidjson::Document msg_json;
						msg_json.Parse(message.c_str());
						if (!msg_json.HasMember("uuid") || !msg_json["uuid"].IsString())
						{
							spdlog::error("MessagingSystem::onFirstMessage: No uuid in json: {}", message);
							condition.notify_one();
							return;
						}
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
					}
				}
				catch (std::exception& e) {
					error_message.append(e.what());
				}
			};

		this->websocket_->setOnMessageCallback(onFirstMessage);

		this->websocket_->start();

		std::mutex mutex;
		std::unique_lock<std::mutex> lock(mutex);
		condition.wait(lock);

		if (!error_message.empty()) {
			return tl::make_unexpected(error_message);
		}

		if (uuid.empty()) {
			return tl::make_unexpected("No uuid received from server");
		}
		this->uuid_ = uuid;

		return nullptr;
	}

	void GeneralWebSocket::stop()
	{

	}

	void GeneralWebSocket::restart(std::shared_ptr<configuration::Configuration> configuration)
	{
		this->configuration_ = configuration;
		this->tryStart();
		this->stop();
	}

	void GeneralWebSocket::terminate()
	{

	}
}
