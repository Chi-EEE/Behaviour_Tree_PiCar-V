#include <iostream>
#include <chrono>
#include <filesystem>
#include <memory>

#include <daemonpp/daemon.hpp>

#include <cpptrace/cpptrace.hpp>

#include <fmt/format.h>

#include <spdlog/sinks/callback_sink.h>

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
            spdlog::critical("Could not load 'rpi_daemon.service'\n");
            return;
        }

        std::string host = reader.GetString("Host", "host", "");

        this->connection_interval = std::chrono::seconds(reader.GetUnsigned("RaspberryPi", "connection_interval", 1));
        std::string car_name = reader.GetString("RaspberryPi", "car_name", "");

        spdlog::info("Started daemon with host: " + host + " and car_name: " + car_name + "\n");

        std::shared_ptr<Configuration> configuration = std::make_shared<Configuration>(Configuration{host, car_name});
        this->any_configuration_empty = host.empty() || car_name.empty();
        if (this->any_configuration_empty)
        {
            spdlog::warn("A property in the configuration is empty, this daemon will not run with an empty property.");
        }

        spdlog::info("Created the Configuration");

        spdlog::info("Starting to create the Sub Systems");

        std::unique_ptr<LidarDevice> lidar_device = getLidarDevice();
        spdlog::info("Created the LidarDevice");

        std::unique_ptr<MessagingSystem> messaging_system = std::make_unique<MessagingSystem>();
        spdlog::info("Created the MessengingSystem");

        std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(std::make_unique<DummyMovementController>());
        // std::unique_ptr<MovementSystem> movement_system = std::make_unique<MovementSystem>(std::make_unique<DeviceMovementController>());
        spdlog::info("Created the MovementSystem");

        std::unique_ptr<PluginManager> plugin_manager = std::make_unique<PluginManager>();
        spdlog::info("Created the PluginManager");

        spdlog::info("Creating the Car System");
        this->car_system = std::make_shared<CarSystem>(
            configuration,
            std::move(lidar_device),
            std::move(messaging_system),
            std::move(movement_system),
            std::move(plugin_manager));

        spdlog::info("Initializing the Car System");
        this->car_system->initialize();
        spdlog::info("Completed initializing the Car System");

        spdlog::info("Starting up the Car System");
        this->car_system->start();
        spdlog::info("Completed starting the Car System");

        spdlog::info("Completed starting up Daemon");
    }

    inline void update()
    {
        if (this->any_configuration_empty)
        {
            return;
        }

        std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
        const bool CAN_CONNECT = !this->car_system->getMessagingSystem()->isConnected() && now - this->last_connected >= this->connection_interval;
        if (CAN_CONNECT)
        {
            this->connect(now);
        }
        this->car_system->update();
    }

    inline void connect(std::chrono::time_point<std::chrono::steady_clock> &now)
    {
        if (!this->attempted_to_reconnect)
        {
            dlog::notice(fmt::format(R"(Going to repeatedly attempt to connect to the WS Server "{}" at {} second intervals.)", this->car_system->getConfiguration()->host, this->connection_interval.count()));
        }
        auto connection_result = this->car_system->tryConnect();
        if (!connection_result.has_value())
        {
            if (!this->attempted_to_reconnect)
            {
                this->attempted_to_reconnect = true;
                dlog::info(connection_result.error());
            }
        }
        else
        {
            dlog::info("Connected to the WS Server.");
        }
        this->last_connected = now;
    }

    void on_update() override
    {
        try
        {
            this->update();
        }
        catch (cpptrace::exception &e)
        {
            dlog::error("Error: " + std::string(e.message()) + "\n");
            std::ostringstream buffer;
            e.trace().print(buffer, cpptrace::isatty(cpptrace::stderr_fileno));
            dlog::error(buffer.str());
        }
        catch (std::exception &e)
        {
            dlog::error("Error: " + std::string(e.what()) + "\n");
        }
    }

    void on_stop() override
    {
        dlog::notice("Terminating the Daemon");
        this->car_system->terminate();
    }

    void on_reload(const INIReader reader) override
    {
        if (reader.ParseError() < 0)
        {
            dlog::alert("Could not load 'rpi_daemon.service'\n");
            return;
        }
        std::string host = reader.GetString("Host", "host", "");
        std::string car_name = reader.GetString("RaspberryPi", "car_name", "");

        dlog::info(fmt::format(R"(Reloading daemon with host: "{}" and car_name: "{}"\n)", host, car_name));

        std::shared_ptr<Configuration> configuration = std::make_shared<Configuration>(Configuration{
            host,
            car_name,
        });
        this->any_configuration_empty = host.empty() || car_name.empty();
        if (this->any_configuration_empty)
        {
            spdlog::warn("A property in the configuration is empty, this daemon will not run with an empty property.");
        }

        this->car_system->setConfiguration(std::move(configuration));

        this->attempted_to_reconnect = false;

        this->car_system->reload();
    }

private:
    std::shared_ptr<CarSystem> car_system;

    // To check if any configuration is empty
    bool any_configuration_empty = false;

    // To print out reconnect message once
    bool attempted_to_reconnect = false;

    std::chrono::seconds connection_interval = std::chrono::seconds(1);

    // This is initialized as 0
    std::chrono::time_point<std::chrono::steady_clock> last_connected;
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
        std::ostringstream buffer;
        e.trace().print(buffer, cpptrace::isatty(cpptrace::stderr_fileno));
        dlog::error(buffer.str());
    }
    catch (std::exception &e)
    {
        dlog::alert(fmt::format("Terminate called after throwing an instance of {}: {}\n", cpptrace::demangle(typeid(e).name()), e.what()));
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
    auto dlog_logger = spdlog::callback_logger_mt("Daemon", [&](const spdlog::details::log_msg &msg)
                                                  {
        std::string msg_str = std::string(msg.payload.data(), msg.payload.size());
        switch (msg.level)
        {
            case spdlog::level::level_enum::trace:
            case spdlog::level::level_enum::info:
                dlog::info(msg_str);
                break;
            case spdlog::level::level_enum::warn:
                dlog::warning(msg_str);
                break;
            case spdlog::level::level_enum::critical:
                dlog::critical(msg_str);
                break;
            case spdlog::level::level_enum::debug:
                dlog::debug(msg_str);
                break;
            case spdlog::level::level_enum::err:
                dlog::error(msg_str);
                break;
            default:
                break;
        } });
    spdlog::set_default_logger(dlog_logger);
    rpi_daemon dmn;
    dmn.set_name("rpi_daemon");
    dmn.set_update_duration(std::chrono::milliseconds(5));
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
        spdlog::info("Found and using Lidar Scanner\n");
        return std::move(maybe_scanner.value());
    }
    else
    {
        spdlog::warn("Unable to connect to the Lidar Scanner, defaulting to LidarDummy\n");
        return std::make_unique<LidarDummy>();
    }
}
