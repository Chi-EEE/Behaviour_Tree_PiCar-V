#include "LidarScanner.h"

LidarScanner::LidarScanner(const std::string &port) : lidar(RPLidar(port))
{
    lidar.stop();
    lidar.stop_motor();
}

LidarScanner::~LidarScanner()
{
    this->disconnect();
}

std::vector<std::vector<ScanPoint>> LidarScanner::scan()
{
    auto points = get_points();
    
    auto shapes = get_shapes(points);

    this->previous_points.push_back(std::move(points));

    return shapes;
}

void LidarScanner::disconnect()
{
    lidar.stop();
    lidar.stop_motor();
    lidar.disconnect();
    this->connected = false;
}

std::vector<ScanPoint> LidarScanner::get_points()
{
    std::function<std::vector<Measure>()> scanGenerator = lidar.iter_scans();
    std::vector<Measure> scan = scanGenerator();
    std::vector<ScanPoint> points;
    for (int i = 1; i < scan.size(); i++)
    {
        const Measure &measure = scan[i];
        float angle = measure.angle;
        float distance = measure.distance;
        float angleInRadians = angle * (3.14159265f / 180.0f);
        float x = distance * std::cos(angleInRadians);
        float y = distance * std::sin(angleInRadians);
        points.push_back({std::move(angle), std::move(distance), std::move(x), std::move(y)});
    }
    return points;
}

std::vector<std::vector<ScanPoint>> LidarScanner::get_shapes(std::vector<ScanPoint> points)
{
    for (int i = 0; i < points.size(); i++) {
        ScanPoint point = points[i];
        std::vector<ScanPoint> shape;
        shape.push_back(point);
        for (int j = i + 1; j < points.size(); j++) {
            ScanPoint next_point = points[j];
            float distance = sqrt(pow(next_point.x - point.x, 2) + pow(next_point.y - point.y, 2) * 1.0);
            if (distance < 0.1) {
                shape.push_back(next_point);
                continue;
            }
            float slope = (next_point.y - point.y) / (next_point.x - point.x);
            int slope_y = (slope * point.x) + 0;
            if (point.y + 60 <= slope_y && point.y - 60 >= slope_y) {
                shape.push_back(next_point);
                continue;
            }
        }
    }
}
