#ifndef RPLIDAR_H
#define RPLIDAR_H

#pragma once

// Made with the help of ChatGPT


/*
#include <RPLidar.h>
#include <memory>
#include <spdlog/spdlog.h>

int main()
{
    using namespace rplidar;
    spdlog::set_level(spdlog::level::debug);

    // auto lidar_result = RPLidar::create("/dev/ttyUSB0"); // For Linux
    // auto lidar_result = RPLidar::create("COM3"); // For Windows
    if (!lidar_result.has_value())
    {
        std::cout << "Unable to open lidar\n";
        return 0;
    }
    auto &lidar = lidar_result.value();
    std::cout << "Connected to lidar\n";

    auto info_result = lidar->get_info();
    if (!info_result.has_value())
    {
        std::cout << "Unable to get value of get_info() " << info_result.error();
        return 0;
    }
    auto &info = info_result.value();
    std::cout << fmt::format("model: {}, firmware: ({}, {}), hardware: {}, serialnumber: {}\n", info.model, info.firmware.first, info.firmware.second, info.hardware, info.serialNumber);

    auto health_result = lidar->get_health();
    if (!health_result.has_value())
    {
        std::cout << "Unable to get value of get_health() "<< health_result.error();
        return 0;
    }
    auto &health = health_result.value();
    std::cout << fmt::format("({}, {})\n", health.status, health.errorCode);

    std::function<std::vector<Measure>()> scanGenerator = lidar->iter_scans();
    for (int i = 0; i < 10; i++)
    {
        std::vector<Measure> scan = scanGenerator();
        std::cout << "Got " << scan.size() << " Measures!\n";
        for (const Measure &measure : scan)
        {
            // Access individual measurements in the scan
            bool newScan = measure.newScan;
            int quality = measure.quality;
            float angle = measure.angle;
            float distance = measure.distance;
        }
    }

    lidar->stop();
    lidar->stop_motor();
    lidar->disconnect();
    return 1;
}

*/

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

#include <tuple>
#include <stdexcept>

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bin_to_hex.h>

#include <tl/expected.hpp>

namespace rplidar
{
    constexpr uint8_t SYNC_BYTE = 0xA5;
    constexpr uint8_t SYNC_BYTE2 = 0x5A;

    constexpr uint8_t GET_INFO_BYTE = 0x50;
    constexpr uint8_t GET_HEALTH_BYTE = 0x52;

    constexpr uint8_t STOP_BYTE = 0x25;
    constexpr uint8_t RESET_BYTE = 0x40;

    enum ScanType
    {
        NORMAL = 0,
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
        {2, "Error"}};

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

    class ExpressPacket
    {
    public:
        static const uint8_t sync1 = 0xa;
        static const uint8_t sync2 = 0x5;

        ExpressPacket(std::vector<uint8_t> data)
        {
            if ((data[0] >> 4) != sync1 || (data[1] >> 4) != sync2)
            {
                throw std::invalid_argument("try to parse corrupted data");
            }

            uint8_t checksum = 0;
            for (size_t i = 2; i < data.size(); i++)
            {
                checksum ^= data[i];
            }

            if (checksum != ((data[0] & 0x0F) + ((data[1] & 0x0F) << 4)))
            {
                throw std::invalid_argument("Invalid checksum");
            }

            new_scan = (data[2] >> 7) & 0x01;
            start_angle = static_cast<float>((data[1] & 0x0F) << 8 | data[2]) / 64.0f;

            for (size_t i = 4; i < data.size(); i += 5)
            {
                distance.push_back(((data[i + 1] >> 2) & 0x3F) | ((data[i] & 0x3F) << 6));
                angle.push_back((((data[i + 3] & 0x0F) + ((data[i + 1] & 0x01) << 4)) / 8.0f) * getSign(data[i + 1]));
                distance.push_back(((data[i + 2] >> 2) & 0x3F) | ((data[i + 1] & 0x3F) << 6));
                angle.push_back((((data[i + 3] >> 4) & 0x0F) + ((data[i + 2] & 0x01) << 4)) / 8.0f * getSign(data[i + 2]));
            }
        }

        static int getSign(uint8_t value)
        {
            return (value & 0x02) ? -1 : 1;
        }

    public:
        std::vector<uint16_t> distance;
        std::vector<float> angle;
        bool new_scan;
        float start_angle;
    };
    /**
     * @brief Class for communicating with RPLidar rangefinder scanners
     *
     */
    class RPLidar
    {
    public:
        static tl::expected<std::unique_ptr<RPLidar>, nullptr_t> create(const std::string &port, uint32_t baudrate = 115200U)
        {
            try
            {
                // Timeout after 60 seconds if nothing read
                auto timeout = serial::Timeout(1000U, 60000U, 0, 1000U, 0);
                std::unique_ptr<serial::Serial> serial = std::make_unique<serial::Serial>(port, baudrate, timeout);
                std::unique_ptr<RPLidar> lidar = std::make_unique<RPLidar>(port, baudrate, std::move(serial));
                return std::move(lidar);
            }
            catch (std::exception &e)
            {
                return tl::make_unexpected(nullptr);
            }
        }

        /**
         * @brief Initialize RPLidar object for communicating with the sensor
         *
         * @param port Serial port name to which sensor is connected
         * @param baudrate Baudrate for serial connection (the default is 115200)
         */
        RPLidar(const std::string &port, uint32_t baudrate, std::unique_ptr<serial::Serial> serial) : port(port), baudrate(baudrate), _serial(std::move(serial)){};

        ~RPLidar() {}

        void disconnect()
        {
            if (!this->_serial->isOpen())
            {
                return;
            }
            this->_serial->close();
        }

        void _set_pwm(int pwm)
        {
            std::string payload;
            payload.push_back(static_cast<uint8_t>(pwm & 0xFF));
            payload.push_back(static_cast<uint8_t>((pwm >> 8) & 0xFF));
            this->_send_payload_cmd(SET_PWM_BYTE, payload);
        }

        void set_motor_speed(int pwm)
        {
            if (0 <= pwm && pwm <= MAX_MOTOR_PWM)
            {
                std::abort();
            }
            this->_motor_speed = pwm;
            if (this->motor_running)
                this->_set_pwm(this->_motor_speed);
        }

        /**
         * @brief Starts sensor motor
         *
         */
        void start_motor()
        {
            spdlog::info("Starting motor");
            // For A1
            this->_serial->setDTR(false);

            // For A2
            this->_set_pwm(this->_motor_speed);
            this->motor_running = true;
        }

        /**
         * @brief Stops sensor motor
         *
         */
        void stop_motor()
        {
            spdlog::info("Stopping motor");
            // For A2
            this->_set_pwm(0);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            // For A1
            this->_serial->setDTR(true);
            this->motor_running = false;
        }

        /**
         * @brief Sends `cmd` command with `payload` to the sensor
         *
         * @param cmd
         * @param payload
         */
        void _send_payload_cmd(uint8_t cmd, const std::string &payload);
        {
            // Calculate the size
            uint8_t size = static_cast<uint8_t>(payload.size());

            // Construct the request string
            std::string req;
            req += static_cast<uint8_t>(SYNC_BYTE);
            req += static_cast<uint8_t>(cmd);
            req += static_cast<uint8_t>(size);
            req += payload;

            // Calculate the checksum
            uint8_t checksum = 0;
            for (const uint8_t &c : req)
            {
                checksum ^= static_cast<uint8_t>(c);
            }

            req += static_cast<uint8_t>(checksum);

            this->_serial->write(req);
            spdlog::debug("Command sent: {}", spdlog::to_hex(req));
        }

        /**
         * @brief Sends `cmd` command to the sensor
         *
         * @param cmd
         */
        void _send_cmd(uint8_t cmd)
        {
            std::string req;
            req += static_cast<uint8_t>(SYNC_BYTE);
            req += static_cast<uint8_t>(cmd);

            this->_serial->write(req);
            spdlog::debug("Command sent: {}", spdlog::to_hex(req));
        }

        /**
         * @brief Reads descriptor packet
         *
         * @return tl::expected<std::tuple<uint8_t, bool, uint8_t>, std::string>
         */
        tl::expected<
            std::tuple<uint8_t, bool, uint8_t>,
            std::string>
        _read_descriptor()
        {
            // Read descriptor packet
            std::array<uint8_t, DESCRIPTOR_LEN> descriptor;
            memset(descriptor.data(), '\0', sizeof(descriptor));
            this->_serial->read(descriptor.data(), DESCRIPTOR_LEN);
            spdlog::debug("Received descriptor: {}", spdlog::to_hex(descriptor));

            if (descriptor.size() != DESCRIPTOR_LEN)
            {
                return tl::make_unexpected("Descriptor length mismatch");
            }
            else if (descriptor[0] != SYNC_BYTE || descriptor[1] != SYNC_BYTE2)
            {
                return tl::make_unexpected("Incorrect descriptor starting bytes");
            }

            bool isSingle = descriptor[5] == 0;
            return std::make_tuple(descriptor[2], isSingle, descriptor[6]);
        }

        /**
         * @brief Reads response packet with length of `dsize` bytes
         *
         * @param dsize
         * @return std::vector<uint8_t>
         */
        std::vector<uint8_t> _read_response(int dsize)
        {
            spdlog::debug("Trying to read response: {} bytes", dsize);

            std::vector<uint8_t> data;
            data.reserve(dsize);

            while (this->_serial->available() < dsize)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            this->_serial->read(data, dsize);

            spdlog::debug("Received data: {}", spdlog::to_hex(data));
            return data;
        }

        std::string convertToHexString(uint8_t value)
        {
            // Convert a uint8_t to a hexadecimal string
            std::stringstream stream;
            stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(value);
            return stream.str();
        }

        /**
         * @brief Get device information
         *
         * @return tl::expected<DeviceInfo, std::string>
         */
        tl::expected<DeviceInfo, std::string> get_info()
        {
            // Check if there's data in the buffer
            if (this->_serial->available() > 0)
            {
                throw std::runtime_error("Data in buffer, you can't have info! Run flush() to empty the buffer.");
            }
            this->_send_cmd(GET_INFO_BYTE);

            uint8_t dsize;
            bool isSingle;
            uint8_t dtype;
            auto descriptor_result = this->_read_descriptor();
            if (!descriptor_result.has_value())
                return tl::make_unexpected(descriptor_result.error());
            std::tie(dsize, isSingle, dtype) = descriptor_result.value();

            // Check response properties
            if (dsize != INFO_LEN)
            {
                return tl::make_unexpected("Wrong get_info reply length");
            }
            if (!isSingle)
            {
                return tl::make_unexpected("Not a single response mode");
            }
            if (dtype != INFO_TYPE)
            {
                return tl::make_unexpected("Wrong response data type");
            }

            // Read the response
            std::vector<uint8_t> raw = this->_read_response(dsize);

            // Convert serial number to a hex string
            std::string serialNumber;
            for (size_t i = 4; i < raw.size(); ++i)
            {
                serialNumber += convertToHexString(raw[i]);
            }

            // Construct the device info struct
            DeviceInfo info;
            info.model = raw[0];
            info.firmware.first = raw[2];
            info.firmware.second = raw[1];
            info.hardware = raw[3];
            info.serialNumber = serialNumber;

            return info;
        }

        /**
         * @brief Get device health state. When the core system detects some
                potential risk that may cause hardware failure in the future,
                the returned status value will be 'Warning'. But sensor can still work
                as normal. When sensor is in the Protection Stop state, the returned
                status value will be 'Error'. In case of warning or error statuses
                non-zero error code will be returned.
         *
         * @return tl::expected<HealthInfo, std::string>
         */
        tl::expected<HealthInfo, std::string> get_health()
        {
            // Check if there's data in the buffer
            if (this->_serial->available() > 0)
            {
                return tl::make_unexpected("Data in buffer, you can't get health info! Run cleanInput() to empty the buffer.");
            }
            spdlog::info("Asking for health");

            this->_send_cmd(GET_HEALTH_BYTE);

            // Read the descriptor
            uint8_t dsize;
            bool isSingle;
            uint8_t dtype;
            auto descriptor_result = this->_read_descriptor();
            if (!descriptor_result.has_value())
                return tl::make_unexpected(descriptor_result.error());
            std::tie(dsize, isSingle, dtype) = descriptor_result.value();

            // Check response properties
            if (dsize != HEALTH_LEN)
            {
                return tl::make_unexpected("Wrong get_health reply length");
            }
            if (!isSingle)
            {
                return tl::make_unexpected("Not a single response mode");
            }
            if (dtype != HEALTH_TYPE)
            {
                return tl::make_unexpected("Wrong response data type");
            }

            // Read the response
            std::vector<uint8_t> raw = this->_read_response(dsize);

            // Extract status and error code
            std::string status = HEALTH_STATUSES[raw[0]];
            int errorCode = (static_cast<int>(raw[1]) << 8) + static_cast<int>(raw[2]);

            return HealthInfo{status, errorCode};
        }

        /**
         * @brief
         * Clean input buffer by reading all available data
         */
        void clean_input()
        {
            if (this->scanning.currently_scanning)
            {
                throw std::runtime_error("Cleaning not allowed during scanning process active!");
            }
            this->_serial->flushInput();
            this->express_trame = 32;
            this->express_data = nullptr;
        }

        /**
         * @brief
         * Stops scanning process, disables laser diode and the measurement
                system, moves sensor to the idle state.
         */
        void stop()
        {
            spdlog::info("Stopping scanning");
            this->_send_cmd(STOP_BYTE);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            this->scanning.currently_scanning = false;
            this->clean_input();
        }

        /**
         * @brief Start the scanning process
         *
         * @param scanType NORMAL, FORCE or EXPRESS
         */
        tl::expected<nullptr_t, std::string> start(ScanType scanType = ScanType::NORMAL)
        {
            if (this->scanning.currently_scanning)
            {
                return tl::make_unexpected("Scanning already running!");
            }

            auto health_info_result = this->get_health();
            if (!health_info_result.has_value())
                return tl::make_unexpected(health_info_result.error());

            HealthInfo healthInfo = health_info_result.value();
            std::string status = healthInfo.status;
            int errorCode = healthInfo.errorCode;
            spdlog::debug("Health status: {} [{}}]", status, errorCode);

            if (status == "Error")
            {
                this->reset();
                health_info_result = this->get_health();
                if (!health_info_result.has_value())
                    return tl::make_unexpected(health_info_result.error());
                healthInfo = health_info_result.value();
                status = healthInfo.status;
                errorCode = healthInfo.errorCode;
                if (status == "Error")
                {
                    return tl::make_unexpected("RPLidar hardware failure. Error code: " + std::to_string(errorCode));
                }
            }
            else if (status == "Warning")
            {
                spdlog::warn("Warning sensor status detected! Error code: {}", errorCode);
            }

            uint8_t cmd = SCAN_TYPE[scanType]["byte"];

            spdlog::warn("starting scan process in {} mode", scanType);

            if (scanType == EXPRESS)
            {
                // Adjust this part according to your payload format
                std::string payload = "\x00\x00\x00\x00\x00";
                this->_send_payload_cmd(cmd, payload);
            }
            else
            {
                this->_send_cmd(cmd);
            }

            uint8_t dsize;
            bool isSingle;
            uint8_t dtype;

            auto descriptor_result = this->_read_descriptor();
            if (!descriptor_result.has_value())
                return tl::make_unexpected(descriptor_result.error());

            std::tie(dsize, isSingle, dtype) = descriptor_result.value();

            if (dsize != SCAN_TYPE[scanType]["size"])
            {
                return tl::make_unexpected("Wrong get_info reply length");
            }
            if (isSingle)
            {
                return tl::make_unexpected("Not a multiple response mode");
            }
            if (dtype != SCAN_TYPE[scanType]["response"])
            {
                return tl::make_unexpected("Wrong response data type");
            }
            this->scanning = ScanInfo{true, dsize, scanType};
            return nullptr;
        }

        /**
         * @brief Resets sensor core, reverting it to a similar state as it has
                just been powered up.
         *
         */
        void reset()
        {
            spdlog::info("Resetting the sensor");
            this->_send_cmd(RESET_BYTE);
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            this->clean_input();
        }
        tl::expected<Measure, std::string> _process_scan(const std::vector<uint8_t> &raw)
        {
            Measure measurementData;

            bool newScan = static_cast<bool>(raw[0] & 0b1);
            bool inversedNewScan = static_cast<bool>((raw[0] >> 1) & 0b1);
            int quality = static_cast<int>(raw[0] >> 2);

            if (newScan == inversedNewScan)
            {
                return tl::make_unexpected("New scan flags mismatch");
            }

            int checkBit = static_cast<int>(raw[1] & 0b1);
            if (checkBit != 1)
            {
                return tl::make_unexpected("Check bit not equal to 1");
            }

            int anglePart1 = static_cast<int>(raw[1] >> 1);
            int anglePart2 = static_cast<int>(raw[2]) << 7;
            float angle = (anglePart1 + anglePart2) / 64.0;

            int distancePart1 = static_cast<int>(raw[3]);
            int distancePart2 = static_cast<int>(raw[4]) << 8;
            float distance = (distancePart1 + distancePart2) / 4.0;

            measurementData.newScan = newScan;
            measurementData.quality = quality;
            measurementData.angle = angle;
            measurementData.distance = distance;

            return measurementData;
        }

        /**
         * @brief Iterate over measures. Note that consumer must be fast enough,
                otherwise data will be accumulated inside buffer and consumer will get
                data with increasing lag.
         *
         * @param scanType
         * @param maxBufMeas int or False if you want unlimited buffer
                    Maximum number of bytes to be stored inside the buffer. Once
                    numbe exceeds this limit buffer will be emptied out.
         * @return std::function<Measure()>
         */
        Measure _process_express_scan(std::unique_ptr<ExpressPacket> &data, float newAngle, int trame)
        {
            Measure measurementData;

            bool newScan = (newAngle < data->start_angle) && (trame == 1);

            float angle = std::fmod((data->start_angle + ((newAngle - data->start_angle) / 32 * trame - data->angle[trame - 1])), 360);
            float distance = data->distance[trame - 1];

            measurementData.newScan = newScan;
            measurementData.quality = 0; // Replace this with your actual quality value
            measurementData.angle = angle;
            measurementData.distance = distance;

            return measurementData;
        }

        std::function<tl::expected<Measure, std::string>()> iter_measures(ScanType scanType = ScanType::NORMAL, int maxBufMeas = 3000)
        {
            if (!this->motor_running)
            {
                this->start_motor();
            }

            if (!this->scanning.currently_scanning)
            {
                this->start(scanType);
            }

            // Define a lambda function to generate measures
            auto generator = [this, scanType, maxBufMeas]() -> tl::expected<Measure, std::string>
            {
                while (true)
                {
                    int dsize = scanning.dsize;

                    if (maxBufMeas != 0)
                    {
                        int dataInBuf = this->_serial->available();
                        if (dataInBuf > maxBufMeas)
                        {
                            spdlog::warn(
                                "Too many bytes in the input buffer: {}/{}. \n"
                                "Cleaning buffer...",
                                dataInBuf, maxBufMeas);
                            this->stop();
                            this->start(scanning.type);
                        }
                    }

                    if (scanType == ScanType::NORMAL)
                    {
                        std::vector<uint8_t> raw = this->_read_response(dsize);
                        return _process_scan(raw);
                    }
                    else if (scanType == EXPRESS)
                    {
                        if (this->express_trame == 32)
                        {
                            this->express_trame = 0;

                            if (this->express_data == nullptr)
                            {
                                spdlog::debug("reading first time bytes");
                                this->express_data = std::make_unique<ExpressPacket>(ExpressPacket(this->_read_response(dsize)));
                            }

                            this->express_old_data = std::move(this->express_data);
                            spdlog::debug("set old_data with start_angle {}", this->express_old_data->start_angle);
                            this->express_data = std::make_unique<ExpressPacket>(ExpressPacket(this->_read_response(dsize)));
                            spdlog::debug("set new_data with start_angle {}", this->express_data->start_angle);
                        }
                        this->express_trame++;
                        spdlog::debug("process scan of frame %d with angle : \n"
                                      "%f and angle new : %f",
                                      this->express_trame,
                                      this->express_old_data->start_angle,
                                      this->express_data->start_angle);
                        Measure measure = _process_express_scan(this->express_old_data, this->express_data->start_angle, this->express_trame);
                        return measure;
                    }
                }
            };

            return generator;
        }

        /**
         * @brief Iterate over scans. Note that consumer must be fast enough,
                otherwise data will be accumulated inside buffer and consumer will get
                data with increasing lag.
         *
         * @param scanType
         * @param maxBufMeas Maximum number of measures to be stored inside the buffer. Once
                    numbe exceeds this limit buffer will be emptied out.
         * @param minLen Minimum number of measures in the scan for it to be yelded.
         * @return std::function<std::vector<Measure>()>
         */
        std::function<std::vector<Measure>()> iter_scans(ScanType scanType = ScanType::NORMAL, int maxBufMeas = 3000, int minLen = 5)
        {
            auto measureIterator = this->iter_measures(scanType, maxBufMeas);

            // Define a lambda function to generate scans
            auto scanGenerator = [measureIterator, &minLen]() -> std::vector<Measure>
            {
                std::vector<Measure> scanList;
                while (true)
                {
                    tl::expected<Measure, std::string> maybe_measure = measureIterator();
                    if (!maybe_measure.has_value())
                    {
                        continue;
                    }
                    Measure measure = maybe_measure.value();
                    bool newScan = measure.newScan;
                    if (newScan)
                    {
                        if (scanList.size() > minLen)
                        {
                            return scanList;
                        }
                        scanList.clear();
                    }
                    if (measure.distance > 0)
                    {
                        scanList.push_back(measure);
                    }
                }
            };
            return scanGenerator;
        }

    private:
        std::unique_ptr<serial::Serial> _serial = nullptr;

        std::string port;
        uint32_t baudrate;

        int _motor_speed = DEFAULT_MOTOR_PWM;
        bool motor_running = false;
        ScanInfo scanning = ScanInfo{false, 0, ScanType::NORMAL};
        int express_trame = 32;

        std::unique_ptr<ExpressPacket> express_data = nullptr;
        std::unique_ptr<ExpressPacket> express_old_data = nullptr;
    };
}

#endif