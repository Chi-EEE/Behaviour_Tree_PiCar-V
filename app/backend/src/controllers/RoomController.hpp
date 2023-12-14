#pragma once
#include <drogon/HttpController.h>

#include <nlohmann/json.hpp>

#include "../room/RoomManager.hpp"

using json = nlohmann::json;

class RoomController : public drogon::HttpController<RoomController>
{
public:
	METHOD_LIST_BEGIN
		ADD_METHOD_TO(RoomController::getRooms, "/v1/api/rooms", drogon::Get);
	METHOD_LIST_END
		void getRooms(const drogon::HttpRequestPtr& req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback);
};

void RoomController::getRooms(const drogon::HttpRequestPtr& req,
	std::function<void(const drogon::HttpResponsePtr&)>&& callback)
{
	json output;
	output["data"] = json::array();
	for (auto& [room_name, room] : RoomManager::instance()->getRooms())
	{
		const json room_json =
		{
			{"id", room_name},
			{"name", room_name},
		};
		output["data"].push_back(room_json);
	}
	auto response = drogon::HttpResponse::newHttpResponse();
	response->setBody(output.dump());
	response->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
	response->setStatusCode(drogon::HttpStatusCode::k200OK);
	callback(response);
}