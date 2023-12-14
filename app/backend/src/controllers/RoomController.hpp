#pragma once
#include <drogon/HttpController.h>

#include <nlohmann/json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "../room/RoomManager.hpp"

using json = nlohmann::json;

class RoomController : public drogon::HttpController<RoomController>
{
public:
	METHOD_LIST_BEGIN
		ADD_METHOD_TO(RoomController::getRooms, "/api/v1/rooms", drogon::Get);
	METHOD_LIST_END
		void getRooms(const drogon::HttpRequestPtr& req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback);
};

void RoomController::getRooms(const drogon::HttpRequestPtr& req,
	std::function<void(const drogon::HttpResponsePtr&)>&& callback)
{
	rapidjson::Document output;
	output.SetObject();
	rapidjson::Value data(rapidjson::kArrayType);

	for (const auto& [room_name, room] : RoomManager::instance()->getRooms())
	{
		rapidjson::Value room_json(rapidjson::kObjectType);
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