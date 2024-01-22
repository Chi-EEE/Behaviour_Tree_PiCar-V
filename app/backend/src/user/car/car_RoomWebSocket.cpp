#include "../../controllers/RoomWebSocket.h"

void RoomWebSocket::handleCarMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
	std::string&& message,
	const drogon::WebSocketMessageType& type)
{
	spdlog::debug("Received a message from car: {} | RoomWebSocket::handleCarMessage", wsConnPtr->peerAddr().toIp());

	auto& user = wsConnPtr->getContextRef<User>();
	try {
		rapidjson::Document message_json;
		message_json.Parse(message.c_str());

		if (message_json.HasParseError()) {
			spdlog::error("Error parsing JSON: {} | RoomWebSocket::handleCarMessage", message_json.GetParseError());
			return;
		}

		rapidjson::Document car_lidar_scan_output_json;
		car_lidar_scan_output_json.SetObject();

		if (message_json.HasMember("data") && message_json["data"].IsArray()) {
			rapidjson::Value new_measure_list(rapidjson::kArrayType);
			const rapidjson::Value& measure_list = message_json["data"];
			if (!measure_list.IsArray()) {
				spdlog::error("Error: 'data' field is not an array. | RoomWebSocket::handleCarMessage");
				return;
			}
			// Check the measure list in case it contains non-object elements
			for (rapidjson::SizeType i = 0; i < measure_list.Size(); i++) {
				const rapidjson::Value& measure_object = measure_list[i];
				if (!measure_object.IsObject()) {
					spdlog::error("Error: 'data' field contains non-object elements. | RoomWebSocket::handleCarMessage");
					return;
				}
				if (!measure_object.HasMember("distance") || !measure_object["distance"].IsDouble()) {
					spdlog::error("Error: 'data' field does not contain the 'distance' field or 'distance' field is not a number. | RoomWebSocket::handleCarMessage");
					return;
				}
				if (!measure_object.HasMember("angle") || !measure_object["angle"].IsDouble()) {
					spdlog::error("Error: 'data' field does not contain the 'angle' field or 'angle' field is not a number. | RoomWebSocket::handleCarMessage");
					return;
				}
				rapidjson::Value new_measure_object(rapidjson::kObjectType);
				new_measure_object.AddMember("distance", measure_object["distance"].GetDouble(), car_lidar_scan_output_json.GetAllocator());
				new_measure_object.AddMember("angle", measure_object["angle"].GetDouble(), car_lidar_scan_output_json.GetAllocator());
				new_measure_list.PushBack(new_measure_object, car_lidar_scan_output_json.GetAllocator());
			}
			car_lidar_scan_output_json.AddMember("data", new_measure_list, car_lidar_scan_output_json.GetAllocator());
		}
		else {
			spdlog::error("Error: 'data' field not found in the input JSON. | RoomWebSocket::handleCarMessage");
			return;
		}

		car_lidar_scan_output_json.AddMember("name", rapidjson::Value().SetString(user.getName().c_str(), car_lidar_scan_output_json.GetAllocator()), car_lidar_scan_output_json.GetAllocator());
		car_lidar_scan_output_json.AddMember("type", "car", car_lidar_scan_output_json.GetAllocator());
		car_lidar_scan_output_json.AddMember("car", "lidar", car_lidar_scan_output_json.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		car_lidar_scan_output_json.Accept(writer);

		this->chat_rooms.publish(user.getChatRoomName(), buffer.GetString());
	}
	catch (const std::exception& e) {
		spdlog::error("Error: {} | RoomWebSocket::handleCarMessage", e.what());
		return;
	}
}