
#include <RPLidar.h>
#include <memory>
#include <spdlog/spdlog.h>

int main()
{
    using namespace rplidar;
    spdlog::set_level(spdlog::level::off);

    // auto lidar_result = RPLidar::create("/dev/ttyUSB0"); // For Linux
    auto lidar_result = RPLidar::create("COM3"); // For Windows
    if (!lidar_result.has_value())
    {
        std::cout << "Unable to open lidar\n";
        return 0;
    }
    auto &lidar = lidar_result.value();
    std::cout << "Connected to lidar\n";

    // auto info_result = lidar->get_info();
    // if (!info_result.has_value())
    // {
    //     std::cout << "Unable to get value of get_info() " << info_result.error();
    //     return 0;
    // }
    // auto &info = info_result.value();
    // std::cout << fmt::format("model: {}, firmware: ({}, {}), hardware: {}, serialnumber: {}\n", info.model, info.firmware.first, info.firmware.second, info.hardware, info.serialNumber);

    // auto health_result = lidar->get_health();
    // if (!health_result.has_value())
    // {
    //     std::cout << "Unable to get value of get_health() "<< health_result.error();
    //     return 0;
    // }
    // auto &health = health_result.value();
    // std::cout << fmt::format("({}, {})\n", health.status, health.errorCode);

    std::function<std::vector<Measure>()> scanGenerator = lidar->iter_scans();
    for (int i = 0; i < 10; i++)
    {
        std::vector<Measure> scan = scanGenerator();
        std::cout << "Got " << scan.size() << " Measures!\n";
        for (const Measure &measure : scan)
        {
            // Access individual measurements in the scan
            bool newScan = measure.newScan;
            int quality = measure.quality;
            float angle = measure.angle;
            float distance = measure.distance;
        }
    }

    lidar->stop();
    lidar->stop_motor();
    lidar->disconnect();
    return 1;
}