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
        this->message_signal_.connect([this](const std::string message)
                                     { this->handleMessage(message); });
        this->on_disconnect_signal_.connect([this](const std::string message)
                                           { this->onDisconnect(message); });
    }

    void MessagingSystem::initializeWebSocket()
    {
        this->websocket_ = std::make_unique<ix::WebSocket>();
        this->websocket_->disableAutomaticReconnection();

        this->websocket_->setUrl("ws://" + this->configuration_->host);
        ix::WebSocketHttpHeaders headers;
        this->websocket_->setExtraHeaders(headers);
    }

    const tl::expected<nullptr_t, std::string> MessagingSystem::tryConnect()
    {
        this->initializeWebSocket();

        auto maybe_uuid = this->getFirstMessage();

        if (!maybe_uuid.has_value())
        {
            return tl::make_unexpected(maybe_uuid.error());
        }

        this->websocket_->setOnMessageCallback(
            std::bind(&MessagingSystem::onMessageCallback, this, std::placeholders::_1));

        this->uuid_ = maybe_uuid.value();

        return nullptr;
    }

    void MessagingSystem::stop()
    {
        if (this->websocket_ != nullptr)
        {
            this->websocket_->stop();
            this->websocket_ = nullptr;
        }
    }

    void MessagingSystem::terminate()
    {
        this->stop();
        ix::uninitNetSystem();
    }

    void MessagingSystem::setConfiguration(std::shared_ptr<configuration::Configuration> configuration)
    {
        this->configuration_ = configuration;
    }

    void MessagingSystem::onMessageCallback(const ix::WebSocketMessagePtr &msg) const
    {
        switch (msg->type)
        {
        case ix::WebSocketMessageType::Close:
        {
            spdlog::info("Disconnected from the Websocket Server; Reason: {}", msg->closeInfo.reason);
            this->on_disconnect_signal_(msg->closeInfo.reason);
            break;
        }
        case ix::WebSocketMessageType::Message:
        {
            this->message_signal_(msg->str);
            break;
        }
        default:
            break;
        }
    }

    void MessagingSystem::onDisconnect(const std::string message)
    {
        this->connected_ = false;
    }

    void MessagingSystem::handleMessage(const std::string &message) const
    {
        rapidjson::Document message_json;
        message_json.Parse(message.c_str());

        if (message_json.HasParseError() || !message_json.IsObject())
        {
            spdlog::error("MessagingSystem::handleMessage: {}", message);
            return;
        }

        if (!message_json.HasMember("type") || !message_json["type"].IsString())
        {
            spdlog::error("MessagingSystem::handleMessage: no type in json: {}", message);
            return;
        }

        spdlog::info("MessagingSystem::handleMessage: Received message: {}", message);

        const std::string type = message_json["type"].GetString();
        try
        {
            switch (utils::hash(type))
            {
            case utils::hash("command"):
                if (!message_json.HasMember("command") || !message_json["command"].IsString())
                {
                    spdlog::error("MessagingSystem::handleMessage: No command in json", message);
                    return;
                }
                this->command_signal_(message, message_json);
                return;
            default:
                spdlog::info("MessagingSystem::handleMessage: unknown command: {}", type);
                return;
            }
        }
        catch (std::exception e)
        {
            spdlog::error("MessagingSystem::handleMessage: {} | {}", message, e.what());
        }
    }

    void MessagingSystem::sendMessage(const std::string &message)
    {
        if (this->websocket_ != nullptr)
            this->websocket_->send(message);
    }

    void MessagingSystem::sendBinary(const std::string &message)
    {
        if (this->websocket_ != nullptr)
            this->websocket_->sendBinary(message);
    }

    tl::expected<std::string, std::string> MessagingSystem::getFirstMessage()
    {
        FirstMessageStruct first_message_struct;

        this->websocket_->setOnMessageCallback([&](const ix::WebSocketMessagePtr &msg)
                                              {
                try {
                    this->onFirstMessage(msg, first_message_struct);
                } catch (std::exception& e) {
                    spdlog::error("MessagingSystem::getFirstMessage: {}", e.what());
                    first_message_struct.condition.notify_one();
                } });

        this->websocket_->start();

        std::mutex mutex;
        std::unique_lock<std::mutex> lock(mutex);
        first_message_struct.condition.wait(lock);

        if (!first_message_struct.error_message.empty())
        {
            return tl::make_unexpected(first_message_struct.error_message);
        }

        return uuid_;
    }

    void MessagingSystem::onFirstMessage(const ix::WebSocketMessagePtr &msg, FirstMessageStruct &first_message_struct)
    {
        switch (msg->type)
        {
        case ix::WebSocketMessageType::Open:
        {
            this->connected_ = true;
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
                first_message_struct.condition.notify_one();
                return;
            }
            first_message_struct.error_message.append(error_json["message"].GetString());
            this->connected_ = false;
            first_message_struct.condition.notify_one();
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
                first_message_struct.condition.notify_one();
                return;
            }
            // Using append instead of assign to avoid copying the string
            first_message_struct.uuid.append(msg_json["uuid"].GetString());
            first_message_struct.condition.notify_one();
            break;
        }
        case ix::WebSocketMessageType::Error:
        {
            first_message_struct.error_message.append(msg->errorInfo.reason.c_str());
            first_message_struct.condition.notify_one();
            break;
        }
        }
    }
};
