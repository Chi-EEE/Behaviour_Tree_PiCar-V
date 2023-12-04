#pragma once
#include <drogon/HttpController.h>

#include <nlohmann/json.hpp>

#include "../room/RoomManager.hpp"

using json = nlohmann::json;

class RoomController : public drogon::HttpController<RoomController>
{
public:
	METHOD_LIST_BEGIN
		ADD_METHOD_TO(RoomController::get_rooms, "/api/rooms", drogon::Get);
	METHOD_LIST_END
		void get_rooms(const drogon::HttpRequestPtr& req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback);
};

void RoomController::get_rooms(const drogon::HttpRequestPtr& req,
	std::function<void(const drogon::HttpResponsePtr&)>&& callback)
{
	json output;
	for (auto& [room_name, room] : RoomManager::instance()->get_rooms())
	{
		output["data"].array().push_back(room_name);
	}
}