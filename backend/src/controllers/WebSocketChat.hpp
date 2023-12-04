#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>
//#include <nlohmann/json-schema.hpp>

#include "../room/RoomManager.hpp"
#include "../room/User.hpp"

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

	void handleCreateRequest(const drogon::HttpRequestPtr&,
		const drogon::WebSocketConnectionPtr&);

	void handleJoinRequest(const drogon::HttpRequestPtr&,
		const drogon::WebSocketConnectionPtr&);

	WS_PATH_LIST_BEGIN
		WS_PATH_ADD("/ws/room", drogon::Get);
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
		this->chat_rooms.publish(user.get_chat_room_name(), message);
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
		this->chat_rooms.publish(user.get_chat_room_name(), message);
		break;
	}
}

void WebSocketChat::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn)
{
	LOG_DEBUG << "websocket closed!";
	auto& user = conn->getContextRef<std::shared_ptr<User>>();
	std::string room_name = user->get_chat_room_name();

	auto& room = RoomManager::instance()->get_room(room_name);
	room->remove_user(user);
	if (room->get_users().empty()) {
		RoomManager::instance()->remove_room(room_name);
	};

	this->chat_rooms.unsubscribe(room_name, user->get_id());
}

void WebSocketChat::handleNewConnection(const drogon::HttpRequestPtr& req,
	const drogon::WebSocketConnectionPtr& conn)
{
	std::string request = req->getParameter("request");
	spdlog::info("New connection from: {} | WebSocketChat::handleNewConnection", conn->peerAddr().toIp());
	if (request == "create") {
		this->handleCreateRequest(req, conn);
		return;
	}
	if (request == "join") {
		this->handleJoinRequest(req, conn);
		return;
	}
	spdlog::error("Invalid request: {} | WebSocketChat::handleNewConnection", request);
	conn->forceClose();
}

inline void WebSocketChat::handleCreateRequest(const drogon::HttpRequestPtr& req, const drogon::WebSocketConnectionPtr& conn)
{
	std::string room_name = req->getParameter("room_name");
	if (RoomManager::instance()->room_exists(room_name)) {
		spdlog::error("Room {} already exists | WebSocketChat::handleCreateRequest", room_name);
		conn->forceClose();
		return;
	}
	auto user = std::make_shared<User>(
		this->chat_rooms.subscribe(room_name,
			[conn](const std::string& topic,
				const std::string& message) {
					(void)topic;
					conn->send(message);
			}),
		room_name
	);
	auto room = std::make_shared<Room>(user);
	RoomManager::instance()->add_room(room_name, room);
	conn->setContext(user);
}

inline void WebSocketChat::handleJoinRequest(const drogon::HttpRequestPtr& req, const drogon::WebSocketConnectionPtr& conn)
{
	std::string room_name = req->getParameter("room_name");
	if (!RoomManager::instance()->room_exists(room_name)) {
		spdlog::error("Room {} does not exist | WebSocketChat::handleJoinRequest", room_name);
		conn->forceClose();
		return;
	}
	auto user = std::make_shared<User>(
		this->chat_rooms.subscribe(room_name,
			[conn](const std::string& topic,
				const std::string& message) {
					(void)topic;
					conn->send(message);
			}),
		room_name
	);
	auto room = RoomManager::instance()->get_room(room_name);
	room->add_user(user);
	conn->setContext(user);
}