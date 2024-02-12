#include <filesystem>
#include <vector>
#include <string>

#include <drogon/HttpAppFramework.h>

#include <spdlog/spdlog.h>

// Controllers must be included before main
#include "controllers/DynamicController.hpp"

#include "controllers/RoomController.hpp"
#include "controllers/RoomWebSocket.h"

#include "behaviour_tree/node/custom/CarCustomNodeParser.hpp"
#include "behaviour_tree/BehaviourTreeParser.hpp"

const std::string host = "127.0.0.1";
const int port = 10001;

const std::string url = "http://" + host + ":" + std::to_string(port);

int main(int argc, char *argv[])
{
	behaviour_tree::BehaviourTreeParser::instance().setCustomNodeParser(std::make_unique<behaviour_tree::node::custom::CarCustomNodeParser>(behaviour_tree::node::custom::CarCustomNodeParser()));

	std::filesystem::path exe_dir = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path();
	std::string config_json_path = exe_dir.string() + "/settings/config.json";
	drogon::app().loadConfigFile(config_json_path);

	spdlog::set_level(spdlog::level::debug);

	drogon::app().setDefaultHandler([&](const drogon::HttpRequestPtr &req,
										std::function<void(const drogon::HttpResponsePtr &)> &&callback)
									{
			std::string path = req->getPath();
			utils::encode(path);
			const std::string redirect_url = url + "/#" + path;
			auto resp = drogon::HttpResponse::newHttpResponse();
			resp->setStatusCode(drogon::k301MovedPermanently);
			std::string body = R"(<html><head><meta http-equiv="refresh" content="0;url=)" + redirect_url + R"("></head><body><p><a href=")" + redirect_url + R"(">Redirect</a></p></body></html>)";
			resp->setBody(std::move(body));
			callback(resp); });

	std::vector<std::pair<std::string, int>> ip_addresses = {
		{host, port},
		{"0.0.0.0", port},
	};

	for (auto &[host, port] : ip_addresses)
	{
		drogon::app().addListener(host, port);
		spdlog::info("Listening on {}:{}", host, port);
	}
	drogon::app().run();
	return 1;
}