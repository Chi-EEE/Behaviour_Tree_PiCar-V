#ifndef RPLIDAR_H
#define RPLIDAR_H

#pragma once

// Made with the help of ChatGPT

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

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bin_to_hex.h>

#include <tl/expected.hpp>

#include "ExpressPacket.hpp"

namespace rplidar {
	constexpr uint8_t SYNC_BYTE = 0xA5;
	constexpr uint8_t SYNC_BYTE2 = 0x5A;

	constexpr uint8_t GET_INFO_BYTE = 0x50;
	constexpr uint8_t GET_HEALTH_BYTE = 0x52;

	constexpr uint8_t STOP_BYTE = 0x25;
	constexpr uint8_t RESET_BYTE = 0x40;

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
		{ScanType::EXPRESS, {{"byte", 0x82}, {"response", 130}, {"size", 84}}} };

	constexpr int DESCRIPTOR_LEN = 7;
	constexpr int INFO_LEN = 20;
	constexpr int HEALTH_LEN = 3;

	constexpr int INFO_TYPE = 4;
	constexpr int HEALTH_TYPE = 6;

	constexpr int MAX_MOTOR_PWM = 1023;
	constexpr int DEFAULT_MOTOR_PWM = 660;
	constexpr uint8_t SET_PWM_BYTE = 0xF0;

	static std::map<int, std::string> HEALTH_STATUSES = {
		{0, "Good"},
		{1, "Warning"},
		{2, "Error"} };

	struct DeviceInfo
	{
		uint8_t model;
		std::pair<uint8_t, uint8_t> firmware;
		uint8_t hardware;
		std::string serialNumber;
	};

	/**
	 * @brief
	 * Health Info for Lidar Scanner
	 */
	struct HealthInfo
	{
		/**
		 * @brief
		 * 'Good', 'Warning' or 'Error' statuses
		 */
		std::string status;
		/**
		 * @brief
		 * The related error code that caused a warning/error.
		 */
		int errorCode;
	};

	struct Measure
	{
		bool newScan;
		int quality;
		double angle;
		double distance;
	};

	/**
	 * @brief Class for communicating with RPLidar rangefinder scanners
	 *
	 */
	class RPLidar
	{
	public:
		static tl::expected<std::unique_ptr<RPLidar>, nullptr_t> create(const std::string& port, uint32_t baudrate = 115200U);
		
		RPLidar(const std::string& port, uint32_t baudrate, std::unique_ptr<serial::Serial> serial);

		~RPLidar();

		void disconnect();

		void _set_pwm(int pwm);

		void set_motor_speed(int pwm);

		void start_motor();

		void stop_motor();

		void _send_payload_cmd(uint8_t cmd, const std::string& payload);

		void _send_cmd(uint8_t cmd);

		tl::expected<
			std::tuple<uint8_t, bool, uint8_t>,
			std::string
		>
			_read_descriptor();

		std::vector<uint8_t> _read_response(int dsize);

		tl::expected<DeviceInfo, std::string> get_info();

		tl::expected<HealthInfo, std::string> get_health();

		void clean_input();

		void stop();

		tl::expected<nullptr_t, std::string> start(ScanType scanType);

		void reset();

		std::function<tl::expected<Measure, std::string>()> iter_measures(ScanType scanType = NORMAL, int maxBufMeas = 3000);

		std::function<std::vector<Measure>()> iter_scans(ScanType scanType = NORMAL, int maxBufMeas = 3000, int minLen = 5);

	private:
		
		std::unique_ptr<serial::Serial> _serial = nullptr;
		
		std::string port;
		uint32_t baudrate;
		
		int _motor_speed = DEFAULT_MOTOR_PWM;
		bool motor_running = false;
		ScanInfo scanning = { false, 0, ScanType::NORMAL };
		int express_trame = 32;
		
		std::unique_ptr<ExpressPacket> express_data = nullptr;
		std::unique_ptr<ExpressPacket> express_old_data = nullptr;
	};
}

#endif