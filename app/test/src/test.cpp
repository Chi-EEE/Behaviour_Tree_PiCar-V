
#include <RPLidar.h>
#include <memory>
#include <spdlog/spdlog.h>

#include "LidarScanner.h"

int main() {
    using namespace car::system::lidar;
    auto maybe_lidar_scanner = LidarScanner::create("COM3");
    if (!maybe_lidar_scanner.has_value()) {
        std::cout << "Unable to open lidar\n";
        return EXIT_FAILURE;
    }
    auto &lidar_scanner = maybe_lidar_scanner.value();
    lidar_scanner->start();
    for (int i = 0; i < 10; i++)
    {
        std::cout << lidar_scanner->getLidarMessage() << std::endl;
    }
    lidar_scanner->terminate();
    return EXIT_SUCCESS;
}

int test()
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

    std::function<std::vector<Measure>()> scanGenerator = lidar->iter_scans();
    for (int i = 0; i < 100; i++)
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