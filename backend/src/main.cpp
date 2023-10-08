#include <iostream>

#include <fmt/format.h>

#include "RPLidar.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    auto lidar = RPLidar("COM3");
    // std::function<std::vector<Measure>()> scanGenerator = lidar.iter_scans(ScanType::NORMAL, 3000, 5);
    // for (int i = 0; i < 10; i++)
    // {
    //     std::vector<Measure> scan = scanGenerator();
    //     std::cout << "Got " << scan.size() << " Measures!";
    //     for (const Measure &measure : scan)
    //     {
    //         // Access individual measurements in the scan
    //         bool newScan = measure.newScan;
    //         int quality = measure.quality;
    //         float angle = measure.angle;
    //         float distance = measure.distance;
    //     }
    // }
    lidar.stop();
    lidar.stop_motor();
    lidar.disconnect();
}