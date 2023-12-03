#ifndef LIDARSCANNER_H
#define LIDARSCANNER_H

#pragma once

#include "LidarStrategy.h"

class LidarScanner : public LidarStrategy
{
public:
    LidarScanner(const std::string &lidar_port);
    std::vector<Measure> scan();

private:
    RPLidar lidar;
};

#endif