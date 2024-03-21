#ifndef JSONCONFIGURATION_CXX
#define JSONCONFIGURATION_CXX

#pragma once

#include <iostream>
#include <fstream>
#include <variant>
#include <optional>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <tl/expected.hpp>

#include "car/configuration/Configuration.h"

namespace car::configuration
{
	class JsonConfiguration
	{
	public:
		JsonConfiguration(std::string exe_dir) : exe_dir(exe_dir)
		{
		}

		void setConfigFilePath(std::string config_file_path)
		{
			this->config_file_path = config_file_path;
		}

		const std::string& getConfigFilePath() const
		{
			return this->config_file_path;
		}

		tl::expected<Configuration, std::string> loadConfiguration()
		{
			assert(this->config_file_path != "");

			std::string config_file_path = exe_dir + "/" + this->config_file_path;
			std::ifstream config_file(config_file_path);
			if (!config_file)
			{
				std::string error_message = fmt::format(R"(Unable to open "{}")", config_file_path);
				return tl::make_unexpected(error_message);
			}
			rapidjson::IStreamWrapper config_stream(config_file);
			rapidjson::Document config_json;
			config_json.ParseStream<rapidjson::kParseCommentsFlag>(config_stream);
			if (config_json.HasParseError())
			{
				std::string error_message = fmt::format(R"(Error parsing config file, code: "{}")", static_cast<int>(config_json.GetParseError()));
				;
				return tl::make_unexpected(error_message);
			}
			try
			{
				std::string host = config_json["host"].GetString();
				return Configuration{ host };
			}
			catch (const std::exception& e)
			{
				std::string error_message = fmt::format(R"(Error accessing JSON values: "{}")", e.what());
				return tl::make_unexpected(error_message);
			}
		}

	private:
		const std::string exe_dir;
		std::string config_file_path;
	};
};

#endif