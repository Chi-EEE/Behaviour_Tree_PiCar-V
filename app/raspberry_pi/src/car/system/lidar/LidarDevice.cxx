#ifndef LidarDevice_CXX
#define LidarDevice_CXX

#pragma once

#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <RPLidar.h>

using namespace rplidar;

namespace car::system::lidar
{
	class LidarDevice
	{
	public:
		virtual void initialize() const = 0;
		virtual void start() const = 0;

		virtual std::vector<Measure> scan() const = 0;

		std::string getLidarMessage()
		{
			rapidjson::Document output_json;
			output_json.SetObject();
			rapidjson::Value data_array(rapidjson::kArrayType);

			this->scan_data = this->scan();

			for (const Measure &measure : this->scan_data)
			{
				rapidjson::Value measure_object(rapidjson::kObjectType);
				measure_object.AddMember("distance", measure.distance, output_json.GetAllocator());
				measure_object.AddMember("angle", measure.angle, output_json.GetAllocator());
				data_array.PushBack(measure_object, output_json.GetAllocator());
			}

			output_json.AddMember("data", data_array, output_json.GetAllocator());

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			output_json.Accept(writer);
			return buffer.GetString();
		};

		virtual void stop() const = 0;
		virtual void terminate() const = 0;
		virtual void disconnect() const = 0;

	private:
		std::vector<Measure> scan_data;
	};
}

#endif