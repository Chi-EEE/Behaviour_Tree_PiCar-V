#ifndef LIDARDUMMY_H
#define LIDARDUMMY_H

#pragma once

#include <fstream>

#include <nlohmann/json.hpp>

#include "LidarStrategy.h"

using json = nlohmann::json;

class LidarDummy : public LidarStrategy
{
public:
    LidarDummy();
    
    std::vector<Measure> scan();

private:

};

#endif