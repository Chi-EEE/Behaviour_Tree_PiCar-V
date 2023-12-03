#ifndef LIDARSTRATEGY_H
#define LIDARSTRATEGY_H

#pragma once

#include <vector>

#include <rplidar/RPLidar.h>

using namespace rplidar;

class LidarStrategy
{
public:
    virtual std::vector<Measure> scan() = 0;

private:

};

#endif