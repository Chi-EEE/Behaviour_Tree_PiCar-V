#ifndef LIDARSCANNER_H
#define LIDARSCANNER_H

#pragma once

#include <unordered_set>

#include "Imath/ImathVec.h"

#include "RPLidar.h"
#include "ScanPoint.hpp"

class LidarScanner
{
public:
    LidarScanner(const std::string &port);
    ~LidarScanner();
    std::vector<std::vector<ScanPoint>> scan();
    void disconnect();
private:
    std::vector<ScanPoint> get_points();
    std::vector<std::vector<ScanPoint>> get_shapes(std::vector<ScanPoint> points);
    std::vector<std::vector<ScanPoint>> previous_points;

    RPLidar lidar;
    bool connected = true;
};

#endif