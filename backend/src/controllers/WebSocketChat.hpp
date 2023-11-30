#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>

#include <spdlog/spdlog.h>
#include "../room/User.h"

#include <nlohmann/json.hpp>
//#include <nlohmann/json-schema.hpp>

using json = nlohmann::json;
//using json_validator = nlohmann::json_schema::json_validator;

class WebSocketChat : public drogon::WebSocketController<WebSocketChat>
{
public:
	virtual void handleNewMessage(const drogon::WebSocketConnectionPtr&,
		std::string&&,
		const drogon::WebSocketMessageType&) override;

	void handleFirstMessage(const drogon::WebSocketConnectionPtr&,
		std::string&&,
		const drogon::WebSocketMessageType&);

	void handleUserMessage(const drogon::WebSocketConnectionPtr&,
		std::string&&,
		const drogon::WebSocketMessageType&);

	void handleCarMessage(const drogon::WebSocketConnectionPtr&,
		std::string&&,
		const drogon::WebSocketMessageType&);

	virtual void handleConnectionClosed(
		const drogon::WebSocketConnectionPtr&) override;

	virtual void handleNewConnection(const drogon::HttpRequestPtr&,
		const drogon::WebSocketConnectionPtr&) override;

	WS_PATH_LIST_BEGIN
		WS_PATH_ADD("/room", drogon::Get);
	WS_PATH_LIST_END
private:
	drogon::PubSubService<std::string> chat_rooms;
};

void WebSocketChat::handleNewMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	auto& user = wsConnPtr->getContextRef<User>();
	if (user.get_type() == UserType::Unknown) {
		this->handleFirstMessage(wsConnPtr, std::move(message), type);
		return;
	}
	switch (user.get_type()) {
	case UserType::User:
	{
		this->handleUserMessage(wsConnPtr, std::move(message), type);
		break;
	}
	case UserType::Car: {
		this->handleCarMessage(wsConnPtr, std::move(message), type);
		break;
	}
	}
}

void WebSocketChat::handleFirstMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	spdlog::info("Received a first message from: {} | WebSocketChat::handleFirstMessage", wsConnPtr->peerAddr().toIp());
	auto& user = wsConnPtr->getContextRef<User>();
	if (type == drogon::WebSocketMessageType::Ping)
	{
		auto j = json::parse(message);
		if (j["type"] == "user") {
			user.set_type(UserType::User);
			spdlog::info("{} is a user", wsConnPtr->peerAddr().toIp());
		}
		else if (j["type"] == "car") {
			user.set_type(UserType::Car);
			spdlog::info("{} is a car", wsConnPtr->peerAddr().toIp());
		}
	}
}


void WebSocketChat::handleUserMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	auto& user = wsConnPtr->getContextRef<User>();
	spdlog::debug("Received a message from user: {} | WebSocketChat::handleUserMessage", wsConnPtr->peerAddr().toIp());
	switch (type) {
	case drogon::WebSocketMessageType::Text:
		chat_rooms.publish(user.get_chat_room_name(), message);
		break;
	}
}

void WebSocketChat::handleCarMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	auto& user = wsConnPtr->getContextRef<User>();
	spdlog::debug("Received a message from car: {} | WebSocketChat::handleCarMessage", wsConnPtr->peerAddr().toIp());
	switch (type) {
	case drogon::WebSocketMessageType::Text:
		chat_rooms.publish(user.get_chat_room_name(), message);
		break;
	}
}

void WebSocketChat::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn)
{
	LOG_DEBUG << "websocket closed!";
	auto& user = conn->getContextRef<User>();
	chat_rooms.unsubscribe(user.get_chat_room_name(), user.get_id());
}

void WebSocketChat::handleNewConnection(const drogon::HttpRequestPtr& req,
	const drogon::WebSocketConnectionPtr& conn)
{
	spdlog::info("New connection from: {} | WebSocketChat::handleNewConnection", conn->peerAddr().toIp());
	User user(
		chat_rooms.subscribe(req->getParameter("room_name"),
			[conn](const std::string& topic,
				const std::string& message) {
					(void)topic;
					conn->send(message);
			}),
		req->getParameter("room_name")
	);
	conn->setContext(std::make_shared<User>(std::move(user)));
}