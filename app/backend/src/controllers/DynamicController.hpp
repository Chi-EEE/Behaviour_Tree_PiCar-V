#pragma once
#include <drogon/HttpController.h>

class DynamicController : public drogon::HttpController<DynamicController>
{
public:
	METHOD_LIST_BEGIN
	ADD_METHOD_TO(DynamicController::loadIndex, "/", drogon::Get);
	METHOD_LIST_END
	void loadIndex(const drogon::HttpRequestPtr &req,
				   std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

void DynamicController::loadIndex(const drogon::HttpRequestPtr &req,
								  std::function<void(const drogon::HttpResponsePtr &)> &&callback)
{
	callback(drogon::HttpResponse::newFileResponse(drogon::app().getDocumentRoot() + "/index.html"));
}