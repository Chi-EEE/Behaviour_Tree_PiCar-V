#include "LidarScanner.h"

LidarScanner::LidarScanner(const std::string &lidar_port) : lidar(RPLidar(port))
{
    lidar.stop();
    lidar.stop_motor();
}

LidarScanner::~LidarScanner()
{
}

std::vector<Measure> LidarScanner::scan()
{
    std::function<std::vector<Measure>()> scanGenerator = lidar.iter_scans();
    return scanGenerator();
}