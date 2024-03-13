#include <iostream>
#include <chrono>
#include <filesystem>
#include <memory>

#include <daemonpp/daemon.hpp>

#include <cpptrace/cpptrace.hpp>

#include <fmt/format.h>

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

std::unique_ptr<LidarDevice> getLidarDevice();

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

        std::string host = reader.GetString("Host", "host", "");

        this->connection_interval = std::chrono::seconds(reader.GetUnsigned("RaspberryPi", "connection_interval", 1));
        std::string car_name = reader.GetString("RaspberryPi", "car_name", "");

        std::shared_ptr<Configuration> configuration = std::make_shared<Configuration>(Configuration{
            host,
            car_name,
        });

        std::unique_ptr<LidarDevice> lidar_device = getLidarDevice();
        std::unique_ptr<MessagingSystem> messaging_system = std::make_unique<MessagingSystem>(MessagingSystem());
        std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(std::make_unique<DeviceMovementController>(DeviceMovementController()));
        std::unique_ptr<PluginManager> plugin_manager = std::make_unique<PluginManager>(PluginManager());

        this->car_system = std::make_unique<CarSystem>(
            configuration,
            std::move(lidar_device),
            std::move(messaging_system),
            std::move(movement_system),
            std::move(plugin_manager));

        this->car_system->start();
    }

    void on_update() override
    {
        std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
        if (!this->car_system->isConnected() && now - this->last_connected >= this->connection_interval)
        {
            if (!this->attempted_to_reconnect) {
                this->attempted_to_reconnect = true;
                dlog::info(fmt::format("Attempting to connect to the WS Server at {}", this->car_system->getConfiguration()->host));
            }
            auto connection_result = this->car_system->tryConnect();
            if (!connection_result.has_value())
            {
                dlog::notice(connection_result.error());
            } else {
                dlog::info("Connected to the WS Server.");
            }
            this->last_connected = now;
        } else {
            this->attempted_to_reconnect = false;
        }
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

        std::string host = reader.GetString("Host", "host", "");
        std::string car_name = reader.GetString("RaspberryPi", "car_name", "");

        std::shared_ptr<Configuration> configuration = std::make_shared<Configuration>(Configuration{
            host,
            car_name,
        });

        this->car_system->setConfiguration(std::move(configuration));
        this->car_system->reload();
    }

private:
    std::unique_ptr<CarSystem> car_system;

    bool attempted_to_reconnect = false;
    std::chrono::seconds connection_interval = std::chrono::seconds(1);
    std::chrono::time_point<std::chrono::steady_clock> last_connected = std::chrono::steady_clock::time_point::min();
};

// From: https://github.com/jeremy-rifkin/cpptrace/blob/c35392d20bbd6fc8faaa0d4b0b8b8576a5c76f77/src/cpptrace.cpp#L378ss
[[noreturn]] void terminate_handler()
{
    try
    {
        auto ptr = std::current_exception();
        if (ptr == nullptr)
        {
            dlog::alert("terminate called without an active exception\n");
        }
        else
        {
            std::rethrow_exception(ptr);
        }
    }
    catch (cpptrace::exception &e)
    {
        dlog::alert(fmt::format("Terminate called after throwing an instance of {}: {}\n", cpptrace::demangle(typeid(e).name()), e.message()));
        // e.trace().print(std::cerr, isatty(stderr_fileno));
    }
    catch (std::exception &e)
    {
        dlog::alert(fmt::format("Terminate called after throwing an instance of {}: {}\n", cpptrace::demangle(typeid(e).name()), e.what()));
        // print_terminate_trace();
    }
}

int main(int argc, const char *argv[])
{
#ifdef __linux
    if (getuid())
    {
        std::cout << "This program will not work properly unless you are root. Please run this program as root using `sudo`.\n";
        return EXIT_FAILURE;
    }
#endif
    std::set_terminate(terminate_handler);
    rpi_daemon dmn;
    dmn.set_name("rpi_daemon");
    dmn.set_update_duration(std::chrono::milliseconds(500));
    dmn.set_cwd("/");
    dmn.run(argc, argv);
    return 0;
}

std::unique_ptr<LidarDevice> getLidarDevice()
{
#ifdef __linux
    auto maybe_scanner = LidarScanner::create("/dev/ttyUSB0");
#else
    auto maybe_scanner = LidarScanner::create("COM3");
#endif
    if (maybe_scanner.has_value())
    {
        return std::move(maybe_scanner.value());
    }
    dlog::error("Unable to connect to the Lidar Scanner, defaulting to LidarDummy\n");
    return std::make_unique<LidarDummy>();
}
