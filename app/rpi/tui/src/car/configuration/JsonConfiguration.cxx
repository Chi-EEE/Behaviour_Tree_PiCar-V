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

        const std::string &getConfigFilePath() const
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
                Configuration configuration{};
                configuration.host = config_json["host"].GetString();
              
                configuration.camera_index = config_json["camera_index"].GetInt();
                configuration.setCameraFps(config_json["camera_fps"].GetInt());
                configuration.use_camera = config_json["use_camera"].GetBool();
              
                configuration.lidar_port = config_json["lidar_port"].GetString();
                if (configuration.lidar_port.empty()) {
#ifdef __linux
                    configuration.lidar_port = "/dev/ttyUSB0";
#else
                    configuration.lidar_port = "COM3";
#endif
                }
                configuration.use_lidar = config_json["use_lidar"].GetBool();

                configuration.behaviour_tree_update_ms_interval = std::chrono::milliseconds(config_json["behaviour_tree_update_ms_interval"].GetInt());
                return configuration;
            }
            catch (const std::exception &e)
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