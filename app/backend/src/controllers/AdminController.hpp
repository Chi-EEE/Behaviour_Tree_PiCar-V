#pragma once
#include <drogon/HttpController.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <fmt/format.h>

#include <effolkronium/random.hpp>

#include <access_private.hpp>

#include "../plugins/code/CodeManager.hpp"

using effolkronium_Random = effolkronium::random_static;

class AdminController : public drogon::HttpController<AdminController>
{
public:
	METHOD_LIST_BEGIN
		ADD_METHOD_TO(AdminController::createRaspberryPiConnectionCode, "/api/v1/admin/create_code", drogon::Post);
	ADD_METHOD_TO(AdminController::useRaspberryPiConnectionCode, "/api/v1/admin/use_code", drogon::Post);
	METHOD_LIST_END
		void createRaspberryPiConnectionCode(const drogon::HttpRequestPtr& req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
	void useRaspberryPiConnectionCode(const drogon::HttpRequestPtr& req,
		std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
};

void AdminController::createRaspberryPiConnectionCode(const drogon::HttpRequestPtr& req,
	std::function<void(const drogon::HttpResponsePtr&)>&& callback) const
{
	drogon::SessionPtr session = req->session();
	session->clear();

	const std::string ip = req->peerAddr().toIp();
	const trantor::Date now = trantor::Date::date();

	session->insert("ip", ip);
	session->insert("admin", true);
	session->insert("authenicated", false);

	CodeManager* code_manager = drogon::app().getPlugin<CodeManager>();
	const std::shared_ptr<RaspberryPiConnectionCode> raspberry_pi_connection_code = code_manager->getRaspberryPiConnectionCode(session);

	session->insert("raspberry_pi_connection_code_id", raspberry_pi_connection_code);

	rapidjson::Document output;
	output.SetObject();

	std::string raspberry_pi_connection_code_id = fmt::format("{:05}", raspberry_pi_connection_code->id);
	std::string raspberry_pi_connection_passcode = fmt::format("{:04}", raspberry_pi_connection_code->passcode);

	output.AddMember("raspberry_pi_connection_code_id", rapidjson::Value(raspberry_pi_connection_code_id.c_str(), output.GetAllocator()), output.GetAllocator());
	output.AddMember("raspberry_pi_connection_passcode", rapidjson::Value(raspberry_pi_connection_passcode.c_str(), output.GetAllocator()), output.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	output.Accept(writer);

	auto response = drogon::HttpResponse::newHttpResponse();
	response->setBody(buffer.GetString());
	response->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
	response->setStatusCode(drogon::HttpStatusCode::k200OK);
	callback(response);
}

ACCESS_PRIVATE_FIELD(drogon::Session, drogon::Session::SessionMap, sessionMap_);

// Raspberry Pi should use this endpoint to authenticate itself
void AdminController::useRaspberryPiConnectionCode(const drogon::HttpRequestPtr& req,
	std::function<void(const drogon::HttpResponsePtr&)>&& callback) const
{
	drogon::SessionPtr session = req->session();
	auto& session_map = access_private::sessionMap_(*session.get());

	if (!session_map.empty()) {
		auto response = drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
		callback(response);
		return;
	}
	auto maybe_raspberry_pi_connection_code_id = req->getOptionalParameter<std::string>("raspberry_pi_connection_code_id");
	if (!maybe_raspberry_pi_connection_code_id.has_value()) {
		auto response = drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
		callback(response);
		return;
	}

	auto maybe_raspberry_pi_connection_passcode = req->getOptionalParameter<std::string>("raspberry_pi_connection_passcode");
	if (!maybe_raspberry_pi_connection_passcode.has_value()) {
		auto response = drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
		callback(response);
		return;
	}

	std::string raspberry_pi_connection_code_id_string = maybe_raspberry_pi_connection_code_id.value();
	std::string raspberry_pi_connection_passcode_string = maybe_raspberry_pi_connection_passcode.value();

	int raspberry_pi_connection_code_id;
	int raspberry_pi_connection_code_passcode;
	try {
		raspberry_pi_connection_code_id = std::stoi(raspberry_pi_connection_code_id_string);
		raspberry_pi_connection_code_passcode = std::stoi(raspberry_pi_connection_passcode_string);
	}
	catch (const std::exception& e) {
		auto response = drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
		callback(response);
		return;
	}

	CodeManager* code_manager = drogon::app().getPlugin<CodeManager>();
	std::optional<drogon::SessionPtr> maybe_admin_session = code_manager->useRaspberryPiConnectionCode(raspberry_pi_connection_code_id, raspberry_pi_connection_code_passcode);

	if (!maybe_admin_session.has_value()) {
		auto response = drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
		callback(response);
		return;
	}

	drogon::SessionPtr admin_session = maybe_admin_session.value();
	auto maybe_already_authenicated = admin_session->getOptional<bool>("authenicated");

	if (!maybe_already_authenicated.has_value() || maybe_already_authenicated.value()) {
		auto response = drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
		callback(response);
		return;
	}

	admin_session->modify<bool>("authenicated", [&](bool& authenicated) {
		authenicated = true;
		}
	);

	auto response = drogon::HttpResponse::newHttpResponse();
	response->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
	response->setStatusCode(drogon::HttpStatusCode::k200OK);
	callback(response);
}