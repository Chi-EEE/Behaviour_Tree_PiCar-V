#pragma once
#include <drogon/HttpController.h>

class DynamicController : public drogon::HttpController<DynamicController>
{
public:
	METHOD_LIST_BEGIN
		ADD_METHOD_TO(DynamicController::load, "/room/{1}", drogon::Get);
	METHOD_LIST_END
		void load(const drogon::HttpRequestPtr& req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback,
			std::string&& room_name);
};

void DynamicController::load(const drogon::HttpRequestPtr& req,
	std::function<void(const drogon::HttpResponsePtr&)>&& callback,
	std::string&& arg1)
{
	// The 'room' page is dynamically generated locally, so we don't have a specific file to serve.
	callback(drogon::HttpResponse::newFileResponse(drogon::app().getDocumentRoot() + "/dynamic.html"));
}