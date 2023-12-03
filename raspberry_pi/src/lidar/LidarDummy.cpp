#include "LidarDummy.h"

LidarDummy::LidarDummy()
{
    std::ifstream scan_file("scan/file.txt");
    //json scan_json = 
}

std::vector<Measure> LidarDummy::scan() const
{
    return std::vector<Measure>();
}
