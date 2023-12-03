#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class room : public drogon::HttpController<room>
{
public:
	METHOD_LIST_BEGIN
		METHOD_ADD(room::load, "/{1}", Get);
	METHOD_LIST_END
		void load(const HttpRequestPtr& req,
			std::function<void(const HttpResponsePtr&)>&& callback,
			std::string&& room_name);
};

void room::load(const HttpRequestPtr& req,
	std::function<void(const HttpResponsePtr&)>&& callback,
	std::string&& room_name)
{
	// The 'room' page is dynamically generated locally, so we don't have a specific file to serve.
	callback(drogon::HttpResponse::newFileResponse("public/dynamic.html"));
}