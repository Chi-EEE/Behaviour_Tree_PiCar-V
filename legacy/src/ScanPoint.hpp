#ifndef SCANPOINT_H
#define SCANPOINT_H

#pragma once

#include <functional>

struct ScanPoint {
    float x;
    float y;
    float angle;
    float distance;
};

struct ScanPointHash {
    std::size_t operator()(const ScanPoint& p) const {
        std::size_t h1 = std::hash<float>{}(p.x);
        std::size_t h2 = std::hash<float>{}(p.y);
        std::size_t h3 = std::hash<float>{}(p.angle);
        std::size_t h4 = std::hash<float>{}(p.distance);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};
#endif