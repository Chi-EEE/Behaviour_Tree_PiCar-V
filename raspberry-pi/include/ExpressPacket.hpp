#ifndef EXPRESSPACKET_H
#define EXPRESSPACKET_H

#pragma once

// Made with the help of ChatGPT

#include <vector>
#include <tuple>
#include <stdexcept>

class ExpressPacket {
public:
    static const uint8_t sync1 = 0xa;
    static const uint8_t sync2 = 0x5;

    ExpressPacket(std::vector<uint8_t> data) {
        if ((data[0] >> 4) != sync1 || (data[1] >> 4) != sync2) {
            throw std::invalid_argument("try to parse corrupted data");
        }

        uint8_t checksum = 0;
        for (size_t i = 2; i < data.size(); i++) {
            checksum ^= data[i];
        }

        if (checksum != ((data[0] & 0x0F) + ((data[1] & 0x0F) << 4))) {
            throw std::invalid_argument("Invalid checksum");
        }

        new_scan = (data[2] >> 7) & 0x01;
        start_angle = static_cast<float>((data[1] & 0x0F) << 8 | data[2]) / 64.0f;

        for (size_t i = 4; i < data.size(); i += 5) {
            distance.push_back(((data[i + 1] >> 2) & 0x3F) | ((data[i] & 0x3F) << 6));
            angle.push_back((((data[i + 3] & 0x0F) + ((data[i + 1] & 0x01) << 4)) / 8.0f) * getSign(data[i + 1]));
            distance.push_back(((data[i + 2] >> 2) & 0x3F) | ((data[i + 1] & 0x3F) << 6));
            angle.push_back((((data[i + 3] >> 4) & 0x0F) + ((data[i + 2] & 0x01) << 4)) / 8.0f * getSign(data[i + 2]));
        }
    }

    static int getSign(uint8_t value) {
        return (value & 0x02) ? -1 : 1;
    }

public:
    std::vector<uint16_t> distance;
    std::vector<float> angle;
    bool new_scan;
    float start_angle;
};

#endif