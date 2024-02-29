#include "car/configuration/Configuration.h"

#include <iostream>
#include <fstream>
#include <variant>
#include <optional>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <tl/expected.hpp>

namespace car::configuration
{
    Configuration::Configuration(std::string exe_dir) : exe_dir(exe_dir)
    {
    }

    void Configuration::setConfigFilePath(std::string config_file_path)
    {
        this->config_file_path = config_file_path;
    }

    const std::string &Configuration::getConfigFilePath() const
    {
        return this->config_file_path;
    }

    tl::expected<nullptr_t, std::string> Configuration::load()
    {
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
            this->ip_address = config_json["ip_address"].GetString();
            if (config_json.HasMember("port"))
            {
                this->port = std::make_optional<int>(config_json["port"].GetInt());
            }
            this->name = config_json["name"].GetString();
            this->room = config_json["room"].GetString();
        }
        catch (const std::exception &e)
        {
            std::string error_message = fmt::format(R"(Error accessing JSON values: "{}")", e.what());
            return tl::make_unexpected(error_message);
        }
        return nullptr;
    }
};
