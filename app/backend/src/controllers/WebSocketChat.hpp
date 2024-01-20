#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

#include "utils/Utility.hpp"

#include "behaviour_tree/BehaviourTreeParser.hpp"

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
#pragma endregion

#pragma region User
	void handleUserMessage(const drogon::WebSocketConnectionPtr&,
		std::string&&,
		const drogon::WebSocketMessageType&);
	void handleBehaviourTree(const drogon::WebSocketConnectionPtr& wsConnPtr, const std::string& message_data, User& user, std::shared_ptr<Room>& room);
	void handleUserCommand(const drogon::WebSocketConnectionPtr&, const std::string&, User&, std::shared_ptr<Room>&);
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
	if (type != drogon::WebSocketMessageType::Text) {
		return;
	}
	auto& user = wsConnPtr->getContextRef<User>();
	switch (user.getType()) {
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

void WebSocketChat::handleUserMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	auto& user = wsConnPtr->getContextRef<User>();
	try {
		std::optional<std::string> maybe_message_type;
		std::optional<std::string> maybe_message_data;

		rapidjson::Document message_json;
		if (!message_json.Parse(message.c_str()).HasParseError()) {
			if (message_json.HasMember("type") && message_json["type"].IsString()) {
				maybe_message_type = std::make_optional<std::string>(message_json["type"].GetString());
			}
			if (message_json.HasMember("data") && message_json["data"].IsString()) {
				maybe_message_data = std::make_optional<std::string>(message_json["data"].GetString());
			}
		}
		if (!maybe_message_type.has_value() || !maybe_message_data.has_value()) {
			spdlog::info("Invalid JSON from {} | WebSocketChat::handleUserMessage", wsConnPtr->peerAddr().toIp());
			return;
		}
		std::string& message_data = maybe_message_data.value();

		auto& room = RoomManager::instance()->getRoom(user.getChatRoomName());
		if (room == nullptr) {
			spdlog::error("Room {} does not exist | WebSocketChat::handleUserMessage", user.getChatRoomName());
			return;
		}

		std::string& message_type = maybe_message_type.value();
		switch (utils::Utility::hash(message_type)) {
		case utils::Utility::hash("message"): {
			utils::Utility::encode(message_data);
			spdlog::info("Received the following message from {}: {} | WebSocketChat::handleUserMessage", wsConnPtr->peerAddr().toIp(), message_data);

			/*
			Returns the following JSON:
			{
				"name": "username",
				"type": "message",
				"data": message_data
			}
			*/
			rapidjson::Document out_json;
			out_json.SetObject();

			rapidjson::Value name;
			name.SetString(user.getName().c_str(), out_json.GetAllocator());
			out_json.AddMember("name", name, out_json.GetAllocator());
			out_json.AddMember("type", "message", out_json.GetAllocator());

			rapidjson::Value data;
			data.SetString(message_data.c_str(), out_json.GetAllocator());
			out_json.AddMember("data", data, out_json.GetAllocator());

			this->chat_rooms.publish(user.getChatRoomName(), [&out_json] {
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				out_json.Accept(writer);
				return buffer.GetString();
				}());
			break;
		}
		case utils::Utility::hash("behaviour_tree"): {
			this->handleBehaviourTree(wsConnPtr, message_data, user, room);
			break;
		}
		case utils::Utility::hash("command"): {
			//this->handleUserCommand(wsConnPtr, message_data, user, room);
			break;
		}
		}
	}
	catch (std::exception& c) {
		spdlog::error("Invalid JSON from {} | WebSocketChat::handleUserMessage", wsConnPtr->peerAddr().toIp());
	}
}

void WebSocketChat::handleBehaviourTree(const drogon::WebSocketConnectionPtr& wsConnPtr, const std::string& message_data, User& user, std::shared_ptr<Room>& room) {
	auto maybe_behaviour_tree = behaviour_tree::BehaviourTreeParser::instance().parseXML(message_data);
	if (!maybe_behaviour_tree.has_value()) {
		spdlog::error("Invalid behaviour tree from {} | WebSocketChat::handleBehaviourTree", wsConnPtr->peerAddr().toIp());
		spdlog::error("Behaviour tree: {} | WebSocketChat::handleBehaviourTree", message_data);
		spdlog::error("Behaviour tree Error: {} | WebSocketChat::handleBehaviourTree", maybe_behaviour_tree.error());
		return;
	}
	auto& behaviour_tree = maybe_behaviour_tree.value();

	auto& car_user = room->getCarUser();
	if (car_user != nullptr) {

		rapidjson::Document out_json;
		out_json.SetObject();

		out_json.AddMember("type", "command", out_json.GetAllocator());
		out_json.AddMember("command", "custom", out_json.GetAllocator());
		out_json.AddMember("custom_type", "behaviour_tree", out_json.GetAllocator());
		rapidjson::Value custom;
		custom.SetString(behaviour_tree->toString().c_str(), out_json.GetAllocator());
		out_json.AddMember("custom", custom, out_json.GetAllocator());
		std::string out_json_string = [&out_json] {
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			out_json.Accept(writer);
			return buffer.GetString();
			}();
			spdlog::info("Sending behaviour tree to car: {} | WebSocketChat::handleBehaviourTree", out_json_string);
			car_user->getConnection()->send(out_json_string);
	}
}

void WebSocketChat::handleUserCommand(const drogon::WebSocketConnectionPtr& wsConnPtr, const std::string& message_data, User& user, std::shared_ptr<Room>& room) {
	/*
	Returns the following JSON:
	{
		"name": "username",
		"type": "command",
		"command": "move",
		"distance": 10
	}
	*/


	//rapidjson::Document out_json;
	//out_json.SetObject();

	//rapidjson::Value name;
	//name.SetString(user.getName().c_str(), out_json.GetAllocator());
	//out_json.AddMember("name", name, out_json.GetAllocator());
	//out_json.AddMember("type", "command", out_json.GetAllocator());

	//auto command_type_list = Utility::split(message_data.c_str(), 0, 1, ' ');
	//if (command_type_list.size() != 1) {
	//	return;
	//}
	//std::string command_type = command_type_list[0];
	//command_type.erase(0, 1); // Remove the slash
	//// TODO: Make this whole section cleaner

	//switch (Utility::hash(command_type)) {
	//case Utility::hash("move"): {
	//	out_json.AddMember("command", "move", out_json.GetAllocator());
	//	auto arguments = Utility::split(message_data.c_str(), 1, 2, ' ');
	//	if (arguments.size() == 1) {
	//		int distance = std::atoi(arguments[0].c_str());
	//		out_json["distance"] = distance;
	//		if (room->getCarUser() != nullptr)
	//			room->getCarUser()->getConnection()->send(out_json.dump());
	//		this->chat_rooms.publish(user.getChatRoomName(), out_json.dump());
	//	}
	//	else {
	//		spdlog::error("The following arguments do not match the command move from {}: {} | WebSocketChat::handleUserCommand", wsConnPtr->peerAddr().toIp(), message_data);
	//	}
	//	break;
	//}
	//case Utility::hash("turn"): {
	//	out_json.AddMember("command", "turn", out_json.GetAllocator());
	//	auto arguments = Utility::split(message_data.c_str(), 1, 2, ' ');
	//	if (arguments.size() == 1) {
	//		int distance = std::atoi(arguments[0].c_str());
	//		out_json["distance"] = distance;
	//		if (room->getCarUser() != nullptr)
	//			room->getCarUser()->getConnection()->send(out_json.dump());
	//		this->chat_rooms.publish(user.getChatRoomName(), out_json.dump());
	//	}
	//	else {
	//		spdlog::error("The arguments do not match the command turn from {}: {} | WebSocketChat::handleUserCommand", wsConnPtr->peerAddr().toIp(), message_data);
	//	}
	//	break;
	//}
	//default:
	//{
	//	spdlog::error("Invalid command {} from {} | WebSocketChat::handleUserCommand", command_type, wsConnPtr->peerAddr().toIp());
	//	break;
	//}
	//}
}

void WebSocketChat::handleCarMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	auto& user = wsConnPtr->getContextRef<User>();
	if (type != drogon::WebSocketMessageType::Text) {
		return;
	}
	// TODO: Prevent Car User from sending unfiltered messages
	spdlog::debug("Received a message from car: {} | WebSocketChat::handleCarMessage", wsConnPtr->peerAddr().toIp());
	json message_json = json::parse(message);

	json out_json;
	out_json["name"] = user.getName();
	out_json["type"] = "car";
	out_json["car"] = "lidar";
	out_json["data"] = message_json["data"];
	this->chat_rooms.publish(user.getChatRoomName(), out_json.dump());
}

void WebSocketChat::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn)
{
	spdlog::info("Connection closed from: {} | WebSocketChat::handleConnectionClosed", conn->peerAddr().toIp());
	if (conn->hasContext()) {
		auto& user = conn->getContextRef<User>();
		std::string room_name = user.getChatRoomName();

		auto& room = RoomManager::instance()->getRoom(room_name);
		room->removeUser(user);
		if (room->isEmpty()) {
			RoomManager::instance()->removeRoom(room_name);
		};

		this->chat_rooms.unsubscribe(room_name, user.getId());
	}
}

void WebSocketChat::handleNewConnection(const drogon::HttpRequestPtr& req,
	const drogon::WebSocketConnectionPtr& conn)
{
	spdlog::info("New connection from: {} | WebSocketChat::handleNewConnection", conn->peerAddr().toIp());
	std::string request = req->getParameter("request");
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

/// <summary>
/// room_name
/// type
/// </summary>
/// <param name="req"></param>
/// <param name="conn"></param>
inline void WebSocketChat::handleCreateRequest(const drogon::HttpRequestPtr& req, const drogon::WebSocketConnectionPtr& conn)
{
	std::string room_name = req->getParameter("room_name");
	utils::Utility::encode(room_name);
	if (room_name.size() < 3) {
		spdlog::info("Room name {} is too short | WebSocketChat::handleCreateRequest", room_name);
		conn->forceClose();
		return;
	}
	if (RoomManager::instance()->hasRoom(room_name)) {
		spdlog::error("Room {} already exists | WebSocketChat::handleCreateRequest", room_name);
		conn->forceClose();
		return;
	}
	spdlog::info("Creating room {} from {} | WebSocketChat::handleCreateRequest", room_name, req->peerAddr().toIp());
	auto user = std::make_shared<User>(
		this->chat_rooms.subscribe(room_name,
			[conn](const std::string& topic,
				const std::string& message) {
					(void)topic;
					conn->send(message);
			}),
		conn,
		room_name,
		UserType::User
	);
	auto room = std::make_shared<Room>(user);
	RoomManager::instance()->addRoom(room_name, room);
	conn->setContext(user);
}

inline void WebSocketChat::handleJoinRequest(const drogon::HttpRequestPtr& req, const drogon::WebSocketConnectionPtr& conn)
{
	std::string room_name = req->getParameter("room_name");
	utils::Utility::encode(room_name);
	if (room_name.size() < 3) {
		spdlog::info("Room name {} is too short | WebSocketChat::handleCreateRequest", room_name);
		conn->forceClose();
		return;
	}
	if (!RoomManager::instance()->hasRoom(room_name)) {
		spdlog::error("Room {} does not exist | WebSocketChat::handleJoinRequest", room_name);
		conn->forceClose();
		return;
	}
	spdlog::info("Joining room {} from {} | WebSocketChat::handleJoinRequest", room_name, req->peerAddr().toIp());
	UserType type = UserType::User;
	if (req->getParameter("type") == "car") {
		type = UserType::Car;
	}
	auto user = std::make_shared<User>(
		this->chat_rooms.subscribe(room_name,
			[conn](const std::string& topic,
				const std::string& message) {
					(void)topic;
					conn->send(message);
			}),
		conn,
		room_name,
		type
	);
	auto room = RoomManager::instance()->getRoom(room_name);
	room->addUser(user);
	if (type == UserType::Car) {
		room->setCarUser(user);
	}
	conn->setContext(user);
}