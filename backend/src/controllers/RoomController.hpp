#pragma once
#include <drogon/HttpController.h>

class RoomController : public drogon::HttpController<RoomController>
{
public:
	METHOD_LIST_BEGIN
		ADD_METHOD_TO(RoomController::load, "/api/room/create?room_name={1}", drogon::Get);
	METHOD_LIST_END
		void load(const drogon::HttpRequestPtr& req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback,
			std::string&& room_name);
};

void RoomController::load(const drogon::HttpRequestPtr& req,
	std::function<void(const drogon::HttpResponsePtr&)>&& callback,
	std::string&& arg1)
{
	// The 'room' page is dynamically generated locally, so we don't have a specific file to serve.
	callback(drogon::HttpResponse::newFileResponse(drogon::app().getDocumentRoot() + "/dynamic.html"));
}