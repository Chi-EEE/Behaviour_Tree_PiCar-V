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

		this->messaging_system->speed_command_signal.connect([this](const SpeedCommand speed_command)
			{ this->set_rear_wheels_speed(speed_command); });

		this->messaging_system->angle_command_signal.connect([this](const AngleCommand angle_command)
			{ this->set_front_wheels_angle(angle_command); });
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

	void CarSystem::start_lidar_device()
	{
		this->lidar_device->start();
	}

	void CarSystem::stop_lidar_device()
	{
		this->lidar_device->stop();
	}

	void CarSystem::set_rear_wheels_speed(const SpeedCommand& speed_command)
	{
		this->movement_system->set_rear_wheels_speed(speed_command);
	}

	void CarSystem::set_front_wheels_angle(const AngleCommand& angle_command)
	{
		this->movement_system->set_front_wheels_angle(angle_command);
	}

	void CarSystem::set_rear_left_wheel_speed(const SpeedCommand& speed_command)
	{
		this->movement_system->set_rear_left_wheel_speed(speed_command);
	}

	void CarSystem::set_rear_right_wheel_speed(const SpeedCommand& speed_command)
	{
		this->movement_system->set_rear_right_wheel_speed(speed_command);
	}

	void CarSystem::set_front_left_wheel_angle(const AngleCommand& angle_command)
	{
		this->movement_system->set_front_left_wheel_angle(angle_command);
	}

	void CarSystem::set_front_right_wheel_angle(const AngleCommand& angle_command)
	{
		this->movement_system->set_front_right_wheel_angle(angle_command);
	}
}