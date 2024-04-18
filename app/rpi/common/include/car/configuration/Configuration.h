#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#pragma once

#include <chrono>
#include <optional>
#include <string>

#include <tl/expected.hpp>

namespace car::configuration
{
    struct Configuration
    {
        std::string host = "127.0.0.1:3000";

        int camera_index = 0;
        void setCameraFps(const int camera_fps)
        {
            this->camera_fps = camera_fps;
            this->camera_fps_interval = 1000 / camera_fps;
        }
        const int getCameraFpsInterval() { return this->camera_fps_interval; }
        bool use_camera = true;

        std::string lidar_port = "";
        bool use_lidar = true;

        std::chrono::milliseconds behaviour_tree_update_ms_interval = std::chrono::milliseconds(100);

    private:
        int camera_fps = 60;
        int camera_fps_interval = 1000;
    };
};

#endif