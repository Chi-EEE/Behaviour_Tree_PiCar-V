#ifndef CAR_H
#define CAR_H

#pragma once

#include <memory>

#include "lidar/LidarStrategy.h"

class Car
{
public:
    Car(std::unique_ptr<LidarStrategy> lidar);
    ~Car();

    void run();

private:
    std::unique_ptr<LidarStrategy> lidar;
};

#endif