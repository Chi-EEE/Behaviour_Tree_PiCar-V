#include "CarSystem.h"

namespace car::system
{
	CarSystem::CarSystem(
		const std::string& websocket_url,
		std::unique_ptr<LidarDevice> lidar_device,
		std::unique_ptr<MessagingSystem> messaging_system,
		std::unique_ptr<MovementSystem> movement_system) : lidar_device(std::move(lidar_device)), messaging_system(std::move(messaging_system)), movement_system(std::move(movement_system))
	{
	}

	CarSystem::~CarSystem()
	{
		this->stop();
	}

	void CarSystem::initialize()
	{
		this->messaging_system->initialize();
		this->lidar_device->initialize();
		this->movement_system->initialize();

		this->messaging_system->speed_command_signal.connect(
			[this](const int speed)
			{ 
				this->setRearWheelsSpeed(speed);
			}
		);

		this->messaging_system->angle_command_signal.connect(
			[this](const float angle)
			{ 
				this->setFrontWheelsAngle(angle); 
			}
		);
	}

	void CarSystem::start()
	{
		if (!this->running) {
			this->messaging_system->start();
			this->lidar_device->start();
			this->running = true;
		}
	}

	void CarSystem::stop()
	{
		if (this->running)
		{
			this->messaging_system->stop();
			this->lidar_device->stop();
			this->lidar_device->disconnect();
			this->running = false;
		}
	}

	void CarSystem::update()
	{
		if (!this->running)
		{
			return;
		}
		json output_json;
		output_json["data"] = json::array();
		this->scan_data = this->lidar_device->scan();
		for (const Measure& measure : this->scan_data)
		{
			output_json["data"].push_back(
				{
					{"distance", measure.distance},
					{"angle", measure.angle},
				});
		}
		this->messaging_system->sendMessage(output_json.dump());
	}

	void CarSystem::startLidarDevice()
	{
		this->lidar_device->start();
	}

	void CarSystem::stopLidarDevice()
	{
		this->lidar_device->stop();
	}

	void CarSystem::setRearWheelsSpeed(const int& speed)
	{
		this->movement_system->setRearWheelsSpeed(speed);
	}

	void CarSystem::setFrontWheelsAngle(const float& angle)
	{
		this->movement_system->setFrontWheelsAngle(angle);
	}

	void CarSystem::setRearLeftWheelSpeed(const int& speed)
	{
		this->movement_system->setRearLeftWheelSpeed(speed);
	}

	void CarSystem::setRearRightWheelSpeed(const int& speed)
	{
		this->movement_system->setRearRightWheelSpeed(speed);
	}

	void CarSystem::setFrontLeftWheelAngle(const float& angle)
	{
		this->movement_system->setFrontLeftWheelAngle(angle);
	}

	void CarSystem::setFrontRightWheelAngle(const float& angle)
	{
		this->movement_system->setFrontRightWheelAngle(angle);
	}

	void CarSystem::setRearWheelDirectionToForwards()
	{
		this->movement_system->setRearWheelDirectionToForwards();
	}
	void CarSystem::setRearWheelDirectionToBackwards()
	{
		this->movement_system->setRearWheelDirectionToBackwards();
	}
}