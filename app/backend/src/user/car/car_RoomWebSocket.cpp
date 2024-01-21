#include "../../controllers/RoomWebSocket.h"

void RoomWebSocket::handleCarMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	auto& user = wsConnPtr->getContextRef<User>();
	if (type != drogon::WebSocketMessageType::Text) {
		return;
	}
	spdlog::debug("Received a message from car: {} | RoomWebSocket::handleCarMessage", wsConnPtr->peerAddr().toIp());

    rapidjson::Document message_json;
    message_json.Parse(message.c_str());

    if (message_json.HasParseError()) {
        spdlog::error("Error parsing JSON: {} | RoomWebSocket::handleCarMessage", message_json.GetParseError());
        return;
    }

    rapidjson::Document out_json;
    out_json.SetObject();

    out_json.AddMember("name", rapidjson::Value().SetString(user.getName().c_str(), out_json.GetAllocator()), out_json.GetAllocator());
    out_json.AddMember("type", "car", out_json.GetAllocator());
    out_json.AddMember("car", "lidar", out_json.GetAllocator());

    if (message_json.HasMember("data") && message_json["data"].IsString()) {
        rapidjson::Value data;
        data.CopyFrom(message_json["data"], out_json.GetAllocator());
        out_json.AddMember("data", data, out_json.GetAllocator());
    }
    else {
        spdlog::error("Error: 'data' field not found in the input JSON. | RoomWebSocket::handleCarMessage");
        return;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    out_json.Accept(writer);

	this->chat_rooms.publish(user.getChatRoomName(), buffer.GetString());
}