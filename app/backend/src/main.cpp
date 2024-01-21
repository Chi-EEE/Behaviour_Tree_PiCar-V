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

const std::string host = "127.0.0.1";
const int port = 8848;

const std::string url = fmt::format("http://{}:{}", host, port);

void init() {
	behaviour_tree::BehaviourTreeParser::instance().setTaskNodeParser(std::make_unique<behaviour_tree::node::task::CarTaskNodeParser>(behaviour_tree::node::task::CarTaskNodeParser()));
	spdlog::set_level(spdlog::level::debug);
	auto config_json_path = boost::dll::program_location().parent_path() / "settings" / "config.json";
	drogon::app().loadConfigFile(config_json_path.string());

	drogon::app().setDefaultHandler([&](const drogon::HttpRequestPtr& req,
		std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
			const std::string redirect_url = url + "/#" + req->getPath();
			auto resp = drogon::HttpResponse::newHttpResponse();
			resp->setStatusCode(drogon::k301MovedPermanently);
			const std::string body = R"(<html><head><meta http-equiv="refresh" content="0;url=)" + redirect_url + R"("></head><body><p><a href=")" + redirect_url + R"(">Redirect</a></p></body></html>)";
			resp->setBody(std::move(body));
			callback(resp);
		}
	);
	std::vector<std::pair<std::string, int>> ip_addresses = {
		{host, port},
		{"0.0.0.0", 8848},
	};
	for (auto& [host, port] : ip_addresses) {
		drogon::app().addListener(host, port);
		spdlog::info("Listening on {}:{}", host, port);
	}
}

void run() {
	drogon::app().run();
}