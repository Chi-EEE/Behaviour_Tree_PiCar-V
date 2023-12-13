#include <filesystem>
#include <vector>
#include <string>

#include <boost/dll.hpp>
#include <drogon/HttpAppFramework.h>

#include <spdlog/spdlog.h>

// Controllers must be included before main
#include "controllers/DynamicController.hpp"

#include "controllers/RoomController.hpp"
#include "controllers/WebSocketChat.hpp"

int main()
{
	spdlog::set_level(spdlog::level::debug);
	auto config_json_path = boost::dll::program_location().parent_path() / "settings" / "config.json";
	drogon::app().loadConfigFile(config_json_path.string());
	drogon::app().setCustom404Page(drogon::HttpResponse::newFileResponse(drogon::app().getDocumentRoot() + "/404.html"));
	std::vector<std::pair<std::string, int>> ip_addresses = {
		{"0.0.0.0", 8848},
		{"127.0.0.1", 8848},
	};
	for (auto& [host, port] : ip_addresses) {
		drogon::app().addListener(host, port);
		spdlog::info("Listening on {}:{}", host, port);
	}
	drogon::app().run();
}