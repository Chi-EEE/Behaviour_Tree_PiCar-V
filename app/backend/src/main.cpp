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

#include "behaviour_tree/node/task/CarTaskNodeParser.hpp"
#include "behaviour_tree/BehaviourTreeParser.hpp"

void init();
void run();

int main()
{
	init();
	run();
}

void init() {
	behaviour_tree::BehaviourTreeParser::instance().setTaskNodeParser(std::make_unique<behaviour_tree::node::task::CarTaskNodeParser>(behaviour_tree::node::task::CarTaskNodeParser()));

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
}

void run() {
	drogon::app().run();
}