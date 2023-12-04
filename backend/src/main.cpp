#include <filesystem>

#include <boost/dll.hpp>
#include <drogon/HttpAppFramework.h>

// Controllers must be included before main
#include "controllers/DynamicController.hpp"
#include "controllers/WebSocketChat.hpp"

int main()
{
	spdlog::set_level(spdlog::level::debug);
	auto config_json_path = boost::dll::program_location().parent_path() / "settings" / "config.json";
	drogon::app().loadConfigFile(config_json_path.string());
	drogon::app().addListener("0.0.0.0", 8848);
	drogon::app().addListener("127.0.0.1", 8848);
	drogon::app().run();
}