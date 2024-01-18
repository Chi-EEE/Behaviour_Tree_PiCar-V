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

int main()
{
	behaviour_tree::BehaviourTreeParser::instance().setTaskNodeParser(std::make_unique<behaviour_tree::node::task::CarTaskNodeParser>(behaviour_tree::node::task::CarTaskNodeParser()));
	auto maybe_behaviour_tree = behaviour_tree::BehaviourTreeParser::instance().parseFileXML("out/behaviourtree.xml");
	 if (maybe_behaviour_tree) {
	 	std::cout << "Behaviour tree parsed successfully\n";
	 	auto &behaviour_tree = maybe_behaviour_tree.value();
	 	std::cout << behaviour_tree->toString();
	 }
	 else {
	 	std::cout << "Behaviour tree parsing failed: " << maybe_behaviour_tree.error() << "\n";
	 }
	/*spdlog::set_level(spdlog::level::debug);
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
	drogon::app().run();*/
}