#ifndef RPLIDAR_H
#define RPLIDAR_H

#pragma once

#include <string>
#include <stdint.h>

#include <serial/serial.h>

#include <iostream>
#include <map>
#include <vector>

// std::this_thread::sleep_for(std::chrono::milliseconds(1));
#include <chrono>
#include <thread>

// convertToHexString
#include <sstream>
#include <iomanip>

// iter_measures
#include <functional>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bin_to_hex.h>

#include "ExpressPacket.hpp"

#define SYNC_BYTE 0xA5
#define SYNC_BYTE2 0x5A

#define GET_INFO_BYTE 0x50
#define GET_HEALTH_BYTE 0x52

#define STOP_BYTE 0x25
#define RESET_BYTE 0x40

enum ScanType
{
    NORMAL,
    FORCE,
    EXPRESS
};

struct ScanInfo
{
    int currently_scanning;
    int dsize;
    ScanType type;
};

static std::map<ScanType, std::map<std::string, uint8_t>> SCAN_TYPE = {
    {ScanType::NORMAL, {{"byte", 0x20}, {"response", 129}, {"size", 5}}},
    {ScanType::FORCE, {{"byte", 0x21}, {"response", 129}, {"size", 5}}},
    {ScanType::EXPRESS, {{"byte", 0x82}, {"response", 130}, {"size", 84}}}};

#define DESCRIPTOR_LEN 7
#define INFO_LEN 20
#define HEALTH_LEN 3

#define INFO_TYPE 4
#define HEALTH_TYPE 6

#define MAX_MOTOR_PWM 1023
#define DEFAULT_MOTOR_PWM 660
#define SET_PWM_BYTE 0xF0

static std::map<int, std::string> HEALTH_STATUSES = {
    {0, "Good"},
    {1, "Warning"},
    {2, "Error"}};

struct DeviceInfo
{
    uint8_t model;
    std::pair<uint8_t, uint8_t> firmware;
    uint8_t hardware;
    std::string serialNumber;
};

struct HealthInfo
{
    std::string status;
    int errorCode;
};

struct Measure
{
    bool newScan;
    int quality;
    double angle;
    double distance;
};

class RPLidar
{
public:
    RPLidar(const std::string &port, uint32_t baudrate = 115200U);
    
    ~RPLidar();

    void disconnect();
    
    void _set_pwm(int pwm);

    void set_motor_speed(int pwm);

    void start_motor();

    void stop_motor();

    void _send_payload_cmd(uint8_t cmd, const std::string &payload);

    void _send_cmd(uint8_t cmd);

    std::tuple<uint8_t, bool, uint8_t> _read_descriptor();

    std::vector<uint8_t> _read_response(int dsize);

    DeviceInfo get_info();

    HealthInfo get_health();

    void clean_input();

    void stop();

    void start(ScanType scanType);

    void reset();

    std::function<Measure()> iter_measures(ScanType scanType = NORMAL, int maxBufMeas = 3000);

    std::function<std::vector<Measure>()> iter_scans(ScanType scanType = NORMAL, int maxBufMeas = 3000, int minLen = 5);

private:
    std::unique_ptr<serial::Serial> _serial = nullptr;
    std::string port;
    uint32_t baudrate;
    int _motor_speed = DEFAULT_MOTOR_PWM;
    bool motor_running = false;
    ScanInfo scanning = {false, 0, ScanType::NORMAL};
    int express_trame = 32;
    std::shared_ptr<ExpressPacket> express_data = nullptr;
    std::shared_ptr<ExpressPacket> express_old_data = nullptr;
};

#endif