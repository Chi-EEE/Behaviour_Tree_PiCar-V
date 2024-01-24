#include "../../controllers/RoomWebSocket.h"

void RoomWebSocket::handleUserMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
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
			spdlog::info("Invalid JSON from {} | RoomWebSocket::handleUserMessage", wsConnPtr->peerAddr().toIp());
			return;
		}
		RoomManager* room_manager = drogon::app().getPlugin<RoomManager>();
		auto& room = room_manager->getRoom(user.getChatRoomName());
		if (room == nullptr) {
			spdlog::error("Room {} does not exist | RoomWebSocket::handleUserMessage", user.getChatRoomName());
			return;
		}

		std::string& message_data = maybe_message_data.value();
		std::string& message_type = maybe_message_type.value();

		switch (utils::Utility::hash(message_type)) {
		case utils::Utility::hash("message"): {
			utils::Utility::encode(message_data);
			spdlog::info("Received the following message from {}: {} | RoomWebSocket::handleUserMessage", wsConnPtr->peerAddr().toIp(), message_data);
			/*
			Returns the following JSON:
			{
				"name": "username",
				"type": "message",
				"data": message_data
			}
			*/
			rapidjson::Document user_message_json;
			user_message_json.SetObject();

			user_message_json.AddMember("name", rapidjson::Value().SetString(user.getName().c_str(), user_message_json.GetAllocator()), user_message_json.GetAllocator());
			user_message_json.AddMember("type", "message", user_message_json.GetAllocator());

			user_message_json.AddMember("data", rapidjson::Value().SetString(message_data.c_str(), user_message_json.GetAllocator()), user_message_json.GetAllocator());

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			user_message_json.Accept(writer);

			this->chat_rooms.publish(user.getChatRoomName(), buffer.GetString());
			break;
		}
		case utils::Utility::hash("behaviour_tree"): {
			this->handleBehaviourTree(wsConnPtr, message_data, user, room);
			break;
		}
		case utils::Utility::hash("command"): {
			break;
		}
		}
	}
	catch (std::exception& c) {
		spdlog::error("Invalid JSON from {} | RoomWebSocket::handleUserMessage", wsConnPtr->peerAddr().toIp());
	}
}

void RoomWebSocket::handleBehaviourTree(const drogon::WebSocketConnectionPtr& wsConnPtr, const std::string& message_data, User& user, std::shared_ptr<Room>& room) {
	auto maybe_behaviour_tree = behaviour_tree::BehaviourTreeParser::instance().parseXML(message_data);
	if (!maybe_behaviour_tree.has_value()) {
		spdlog::error("Recieved an inavlid behaviour tree from {} | {} | RoomWebSocket::handleBehaviourTree", wsConnPtr->peerAddr().toIp(), maybe_behaviour_tree.error());
		return;
	}
	auto& behaviour_tree = maybe_behaviour_tree.value();
	spdlog::info("Recieved a behaviour tree from {} | {} | RoomWebSocket::handleBehaviourTree", wsConnPtr->peerAddr().toIp(), behaviour_tree->toString());

	auto& car_user = room->getCarUser();
	if (car_user != nullptr)
	{
		rapidjson::Document behaviour_command_json;
		behaviour_command_json.SetObject();

		behaviour_command_json.AddMember("type", "command", behaviour_command_json.GetAllocator());
		behaviour_command_json.AddMember("command", "custom", behaviour_command_json.GetAllocator());
		behaviour_command_json.AddMember("custom_type", "behaviour_tree", behaviour_command_json.GetAllocator());
		behaviour_command_json.AddMember("custom", rapidjson::Value().SetString(behaviour_tree->toString().c_str(), behaviour_command_json.GetAllocator()), behaviour_command_json.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		behaviour_command_json.Accept(writer);

		std::string behaviour_command_json_string = buffer.GetString();

		spdlog::info("Sending behaviour tree to car: {} | RoomWebSocket::handleBehaviourTree", behaviour_command_json_string);
		car_user->getConnection()->send(behaviour_command_json_string);
	}
	else {
		spdlog::error("Attempt to send behaviour tree to car user but it was not found | RoomWebSocket::handleBehaviourTree");
	}
}
