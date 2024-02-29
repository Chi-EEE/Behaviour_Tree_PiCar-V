#include "car/system/lidar/LidarDevice.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

namespace car::system::lidar
{
    std::string LidarDevice::getLidarMessage()
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
}
