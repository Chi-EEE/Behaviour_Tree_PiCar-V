#ifndef LIDARSCANNER_H
#define LIDARSCANNER_H

#pragma once

#include "LidarStrategy.h"

#include <memory>
#include <rplidar/RPLidar.h>

using namespace rplidar;

class LidarScanner : public LidarStrategy
{
public:
    LidarScanner(const std::string &lidar_port);

    std::vector<Measure> scan() const override;

private:
    void initialize() const;

    std::unique_ptr<RPLidar> lidar;
};

#endif