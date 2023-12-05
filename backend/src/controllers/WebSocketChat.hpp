#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

#include "../room/RoomManager.hpp"
#include "../room/User.hpp"

using json = nlohmann::json;

class WebSocketChat : public drogon::WebSocketController<WebSocketChat>
{
public:
#pragma region Message
	virtual void handleNewMessage(const drogon::WebSocketConnectionPtr&,
		std::string&&,
		const drogon::WebSocketMessageType&) override;

	void handleFirstMessage(const drogon::WebSocketConnectionPtr&,
		std::string&&,
		const drogon::WebSocketMessageType&);

#pragma endregion

#pragma region User
	void handleUserMessage(const drogon::WebSocketConnectionPtr&,
		std::string&&,
		const drogon::WebSocketMessageType&);

	void handleUserTextMessage(const drogon::WebSocketConnectionPtr&, User& user,
		std::string&& message);
#pragma endregion

#pragma region Car
	void handleCarMessage(const drogon::WebSocketConnectionPtr&,
		std::string&&,
		const drogon::WebSocketMessageType&);
#pragma endregion

#pragma region General
	virtual void handleConnectionClosed(
		const drogon::WebSocketConnectionPtr&) override;

	virtual void handleNewConnection(const drogon::HttpRequestPtr&,
		const drogon::WebSocketConnectionPtr&) override;

	void handleCreateRequest(const drogon::HttpRequestPtr&,
		const drogon::WebSocketConnectionPtr&);

	void handleJoinRequest(const drogon::HttpRequestPtr&,
		const drogon::WebSocketConnectionPtr&);
#pragma endregion
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

void santize_name(std::string& name) {
	for (int i = 0; i < name.size(); i++) {
		if (name[i] < 'A' || name[i] > 'Z' && name[i] < 'a'
			|| name[i] > 'z') {
			name.erase(i, 1);
			i--;
		}
	}
};

void WebSocketChat::handleFirstMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	spdlog::info("Received a first message from: {} | WebSocketChat::handleFirstMessage", wsConnPtr->peerAddr().toIp());
	auto& user = wsConnPtr->getContextRef<User>();
	if (type == drogon::WebSocketMessageType::Ping)
	{
		auto j = json::parse(message);
		std::string name = j["name"].get<std::string>();
		santize_name(name);
		name.resize(20);
		user.set_name(name);
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
	{

		break;
	}
	}
}

// https://stackoverflow.com/a/46931770
std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}

void WebSocketChat::handleUserTextMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	User& user,
	std::string&& message) {
	json message_json = json::parse(message);
	const std::string message_data = message_json["data"].get<std::string>();

	auto& room = RoomManager::instance()->get_room(user.get_chat_room_name());

	json out_json;
	out_json["name"] = user.get_name();
	bool is_command = message_data.rfind("/", 0);
	if (is_command) {
		out_json["type"] = "command";
		size_t count;
		auto split_string = split(message_data.c_str(), ' ');
		std::string command_type = split_string[0];
		if (command_type == "move") {
			out_json["command"] = "move";
			if (count == 2) {
				int distance = std::atoi(split_string[1].c_str());
				out_json["distance"] = distance;
				room->get_car_user()->get_connection()->send(out_json.dump());
			}
		}
		else if (command_type == "turn") {

		}
		else if (command_type == "stop") {

		}
		else if (command_type == "speed") {

		}
	}
	else {
		out_json["type"] = "message";
	}
	this->chat_rooms.publish(user.get_chat_room_name(), out_json.dump());
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
	spdlog::error("Invalid request '{}' from {} | WebSocketChat::handleNewConnection", request, conn->peerAddr().toIp());
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
		room_name,
		conn
	);
	user->set_type(UserType::User);
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
		room_name,
		conn
	);
	auto room = RoomManager::instance()->get_room(room_name);
	room->add_user(user);
	conn->setContext(user);
}