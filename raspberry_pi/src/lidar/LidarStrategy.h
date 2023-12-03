#ifndef LIDARSTRATEGY_H
#define LIDARSTRATEGY_H

#pragma once

#include <vector>

#include <rplidar/RPLidar.h>

using namespace rplidar;

class LidarStrategy
{
public:
    virtual void start() const = 0;
    virtual std::vector<Measure> scan() const = 0;

private:

};

#endif