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

		this->messaging_system->speed_command_signal.connect([this](const SpeedCommand speed_command)
			{ this->setRearWheelsSpeed(speed_command); });

		this->messaging_system->angle_command_signal.connect([this](const AngleCommand angle_command)
			{ this->setFrontWheelsAngle(angle_command); });
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
		if (!this->running)
		{
			this->messaging_system->stop();
			this->lidar_device->stop();
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

	void CarSystem::setRearWheelsSpeed(const SpeedCommand& speed_command)
	{
		this->movement_system->setRearWheelsSpeed(speed_command);
	}

	void CarSystem::setFrontWheelsAngle(const AngleCommand& angle_command)
	{
		this->movement_system->setFrontWheelsAngle(angle_command);
	}

	void CarSystem::setRearLeftWheelSpeed(const SpeedCommand& speed_command)
	{
		this->movement_system->setRearLeftWheelSpeed(speed_command);
	}

	void CarSystem::setRearRightWheelSpeed(const SpeedCommand& speed_command)
	{
		this->movement_system->setRearRightWheelSpeed(speed_command);
	}

	void CarSystem::setFrontLeftWheelAngle(const AngleCommand& angle_command)
	{
		this->movement_system->setFrontLeftWheelAngle(angle_command);
	}

	void CarSystem::setFrontRightWheelAngle(const AngleCommand& angle_command)
	{
		this->movement_system->setFrontRightWheelAngle(angle_command);
	}
}