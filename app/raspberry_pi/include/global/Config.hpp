#ifndef CONFIG_CXX
#define CONFIG_CXX

#pragma once

#include <iostream>
#include <fstream>
#include <variant>
#include <optional>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <spdlog/spdlog.h>

#define GET_CONFIG_VALUE(x) global::Config::getInstance().x

namespace global
{
	/**
	 * @brief Singleton class for the global configuration
	 *
	 */
	class Config
	{
	public:
		Config(Config const&) = delete;
		Config& operator=(Config const&) = delete;
		static Config& getInstance()
		{
			static Config instance;
			return instance;
		}
	public:
		std::string lidar_port;

		std::string host;
		std::optional<int> port = std::nullopt;

		std::string name;
		std::string room;
	private:
		Config()
		{
			this->load();
		}
		void load()
		{
			std::ifstream config_file("settings/config.jsonc");
			if (!config_file)
			{
				spdlog::error("Unable to open 'settings/config.jsonc'");
				throw new std::runtime_error("Unable to open 'settings/config.jsonc'");
			}
			rapidjson::IStreamWrapper config_stream(config_file);
			rapidjson::Document config_json;
			config_json.ParseStream<rapidjson::kParseCommentsFlag>(config_stream);
			if (config_json.HasParseError()) {
				spdlog::error("Error parsing config file. Error code: {}", static_cast<int>(config_json.GetParseError()));
				throw new std::runtime_error("Error parsing config file. Error code: " + std::to_string(static_cast<int>(config_json.GetParseError())));
			}
			try {
				this->lidar_port = config_json["lidar_port"].GetString();
				this->host = config_json["host"].GetString();
				if (config_json.HasMember("port")) {
					this->port = std::make_optional<int>(config_json["port"].GetInt());
				}
				this->name = config_json["name"].GetString();
				this->room = config_json["room"].GetString();
			}
			catch (const std::exception& e) {
				spdlog::error("Error accessing JSON values: {}", e.what());
				throw new std::runtime_error("Error accessing JSON values: " + std::string(e.what()));
			}
		}
	};
};

#endif