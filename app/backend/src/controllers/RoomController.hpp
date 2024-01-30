#pragma once
#include <drogon/HttpController.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "../room/RoomManager.hpp"

class RoomController : public drogon::HttpController<RoomController>
{
public:
	METHOD_LIST_BEGIN
	ADD_METHOD_TO(RoomController::getRooms, "/api/v1/rooms", drogon::Get);
	METHOD_LIST_END
	void getRooms(const drogon::HttpRequestPtr &req,
				  std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;
};

void RoomController::getRooms(const drogon::HttpRequestPtr &req,
							  std::function<void(const drogon::HttpResponsePtr &)> &&callback) const
{
	rapidjson::Document output;
	output.SetObject();
	rapidjson::Value data(rapidjson::kArrayType);

	RoomManager *room_manager = drogon::app().getPlugin<RoomManager>();
	for (const auto &[room_name, room] : room_manager->getRooms())
	{
		rapidjson::Value room_json(rapidjson::kObjectType);
		// TODO: Add room id
		room_json.AddMember("id", rapidjson::Value().SetString(room_name.c_str(), output.GetAllocator()), output.GetAllocator());
		room_json.AddMember("name", rapidjson::Value().SetString(room_name.c_str(), output.GetAllocator()), output.GetAllocator());
		data.PushBack(room_json, output.GetAllocator());
	}
	output.AddMember("data", data, output.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	output.Accept(writer);

	auto response = drogon::HttpResponse::newHttpResponse();
	response->setBody(buffer.GetString());
	response->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
	response->setStatusCode(drogon::HttpStatusCode::k200OK);
	callback(response);
}