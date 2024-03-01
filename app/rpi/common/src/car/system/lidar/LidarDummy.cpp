#include "car/system/lidar/LidarDummy.h"

#include <spdlog/spdlog.h>

namespace car::system::lidar
{
    LidarDummy::LidarDummy()
    {
        spdlog::warn("Currently using the LidarDummy");
    };
}
