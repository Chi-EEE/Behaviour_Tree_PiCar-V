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

        std::string ip_address = reader.GetString("Host", "ip_address", "");
        std::optional<int> port = std::nullopt;
        if (reader.HasValue("Host", "port"))
        {
            port = reader.GetInteger("Host", "port", 0);
        }
        std::string rpi_name = reader.GetString("RaspberryPi", "name", "");
        std::string room_name = reader.GetString("Room", "name", "");

        std::shared_ptr<Configuration> configuration = std::make_shared<Configuration>(Configuration{
            ip_address,
            port,
            rpi_name,
            room_name,
        });

        std::unique_ptr<LidarDevice> lidar_device = getLidarDevice(true);
        std::unique_ptr<MessagingSystem> messaging_system = std::make_unique<MessagingSystem>(MessagingSystem());
        std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(std::make_unique<DeviceMovementController>(DeviceMovementController()));
        std::unique_ptr<PluginManager> plugin_manager = std::make_unique<PluginManager>(PluginManager());

        this->car_system = std::make_unique<CarSystem>(
            configuration,
            std::move(lidar_device),
            std::move(messaging_system),
            std::move(movement_system),
            std::move(plugin_manager));
    }

    void on_update() override
    {
        this->car_system->update();
    }

    void on_stop() override
    {
        dlog::info("Stopping rpi_daemon\n");
		this->car_system->terminate();
    }

    void on_reload(const INIReader reader) override
    {
        if (reader.ParseError() < 0)
        {
            dlog::alert("Could not load 'rpi_daemon.service'\n");
            return;
        }
        dlog::info("Reloading rpi_daemon\n");
        
        std::string ip_address = reader.GetString("Host", "ip_address", "");
        std::optional<int> port = std::nullopt;
        if (reader.HasValue("Host", "port"))
        {
            port = reader.GetInteger("Host", "port", 0);
        }
        std::string rpi_name = reader.GetString("rpi", "name", "");
        std::string room_name = reader.GetString("room", "name", "");

        std::shared_ptr<Configuration> configuration = std::make_shared<Configuration>(Configuration{
            ip_address,
            port,
            rpi_name,
            room_name,
        });

        this->car_system->setConfiguration(std::move(configuration));
        this->car_system->reload();
    }

private:
    std::unique_ptr<CarSystem> car_system;
};

int main(int argc, const char *argv[])
{
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