#include "Car.h"

Car::Car(std::unique_ptr<LidarStrategy> lidar) : lidar(std::move(lidar))
{
}

Car::~Car()
{
}

void Car::run()
{
    while (true)
    {
        this->lidar->scan();
    }
}
