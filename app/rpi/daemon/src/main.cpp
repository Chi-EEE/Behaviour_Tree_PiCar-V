#include <iostream>
#include <chrono>
#include <filesystem>
#include <memory>

#include <daemonpp/daemon.hpp>

#include "car/system/CarSystem.h"

#include "car/system/lidar/LidarScanner.h"
#include "car/system/lidar/LidarDummy.h"

#include "car/system/movement/controller/DummyMovementController.h"
#include "car/system/movement/controller/DeviceMovementController.h"

#include "car/plugin/PluginManager.h"

using namespace daemonpp;

using namespace car::system;
using namespace car::system::lidar;
using namespace car::system::movement::controller;

using namespace car::configuration;
using namespace car::plugin;

std::unique_ptr<LidarDevice> getLidarDevice(bool dummy);

static std::string s_exe_dir;

class rpi_daemon : public daemon
{
public:
    void on_start(const INIReader reader) override
    {
        if (reader.ParseError() < 0)
        {
            dlog::alert("Could not load 'rpi_daemon.service'\n");
            return;
        }
        dlog::info("Starting rpi_daemon\n");

        std::shared_ptr<Configuration> configuration = std::make_shared<Configuration>(Configuration(s_exe_dir));
        configuration->setConfigFilePath("settings/config.jsonc");
        configuration->load();
        
        std::unique_ptr<LidarDevice> lidar_device = getLidarDevice(true);
        std::unique_ptr<MessagingSystem> messaging_system = std::make_unique<MessagingSystem>(MessagingSystem(configuration));
        std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(DeviceMovementController());
        std::unique_ptr<PluginManager> plugin_manager = std::make_unique<PluginManager>(PluginManager());

        this->car_system = std::make_unique<CarSystem>();
    }
    void on_update() override
    {
    }
    void on_stop() override
    {
    }
    void on_reload(const INIReader reader) override
    {
    }

private:
    std::unique_ptr<CarSystem> car_system;
};

int main(int argc, const char *argv[])
{
    s_exe_dir = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path().string();

    rpi_daemon dmn;
    dmn.set_name("rpi_daemon");
    dmn.set_update_duration(std::chrono::milliseconds(500));
    dmn.set_cwd("/");
    dmn.run(argc, argv);
    return 0;
}

std::unique_ptr<LidarDevice> getLidarDevice(bool dummy)
{
    if (dummy)
    {
        return std::make_unique<LidarDummy>();
    }
    else
    {
#ifdef __linux
        auto maybe_scanner = LidarScanner::create("/dev/ttyUSB0");
#else
        auto maybe_scanner = LidarScanner::create("COM3");
#endif
        if (!maybe_scanner.has_value())
        {
            spdlog::error("Unable to connect to the Lidar Scanner");
            throw std::runtime_error("Unable to connect to the Lidar Scanner");
        }
        return std::move(maybe_scanner.value());
    }
}