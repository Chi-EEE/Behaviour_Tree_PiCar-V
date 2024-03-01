#include "car/system/CarSystem.h"

#include <memory>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "car/configuration/Configuration.h"

#include "car/system/lidar/LidarDevice.h"
#include "car/system/messaging/MessagingSystem.h"
#include "car/system/movement/MovementSystem.h"

#include "car/plugin/PluginManager.h"

namespace car::system
{
	CarSystem::CarSystem(
		std::shared_ptr<Configuration> configuration,
		std::unique_ptr<LidarDevice> lidar_device,
		std::unique_ptr<MessagingSystem> messaging_system,
		std::unique_ptr<MovementSystem> movement_system,
		std::unique_ptr<PluginManager> plugin_manager) : configuration(configuration),
														 lidar_device(std::move(lidar_device)),
														 messaging_system(std::move(messaging_system)),
														 movement_system(std::move(movement_system)),
														 plugin_manager(std::move(plugin_manager))
	{
	}

	void CarSystem::initialize()
	{
		this->messaging_system->initialize(this->configuration);
		this->lidar_device->initialize();
		this->movement_system->initialize();
		this->plugin_manager->initialize(shared_from_this());

		this->messaging_system->speed_command_signal.connect(
			[this](const int speed)
			{
				this->setRearWheelsSpeed(speed);
			});

		this->messaging_system->angle_command_signal.connect(
			[this](const float angle)
			{
				this->setFrontWheelsAngle(angle);
			});
	}

	void CarSystem::connectToServer()
	{
		if (!this->connectedToServer)
		{
			this->connectedToServer = true;
			this->messaging_system->start();
			this->lidar_device->start();
			this->movement_system->start();
		}
	}

	void CarSystem::disconnectFromServer()
	{
		if (this->connectedToServer)
		{
			this->connectedToServer = false;
			this->messaging_system->stop();
			this->lidar_device->stop();
			this->movement_system->stop();
			this->plugin_manager->stop();
		}
	}

	/// <summary>
	/// Only devices should be terminated here since deconstructor does not work when the program is terminated by the user.
	/// </summary>
	void CarSystem::terminate()
	{
		this->connectedToServer = false;
		this->messaging_system->terminate();
		this->lidar_device->terminate();
		this->movement_system->terminate();
		this->plugin_manager->terminate();
	}

	void CarSystem::update()
	{
		if (this->connectedToServer)
		{
			this->messaging_system->sendMessage(this->lidar_device->getLidarMessage());
		}
		this->plugin_manager->update();
	}

	void CarSystem::setConfiguration(std::shared_ptr<Configuration> configuration)
	{
		this->configuration = configuration;
	}

	void CarSystem::startLidarDevice()
	{
		this->lidar_device->start();
	}

	void CarSystem::stopLidarDevice()
	{
		this->lidar_device->stop();
	}

	void CarSystem::setRearWheelsSpeed(const int &speed)
	{
		this->movement_system->setRearWheelsSpeed(speed);
	}
	void CarSystem::setRearLeftWheelSpeed(const int &speed)
	{
		this->movement_system->setRearLeftWheelSpeed(speed);
	}

	void CarSystem::setRearRightWheelSpeed(const int &speed)
	{
		this->movement_system->setRearRightWheelSpeed(speed);
	}

	void CarSystem::setFrontWheelsAngle(const float &angle)
	{
		this->movement_system->setFrontWheelsAngle(angle);
	}

	void CarSystem::setCameraServo1Angle(const float &angle)
	{
		this->movement_system->setCameraServo1Angle(angle);
	}
	void CarSystem::setCameraServo2Angle(const float &angle)
	{
		this->movement_system->setCameraServo2Angle(angle);
	}

	void CarSystem::setRearWheelsDirectionToForward()
	{
		this->movement_system->setRearWheelsDirectionToForward();
	}

	void CarSystem::setRearLeftWheelDirectionToForward()
	{
		this->movement_system->setRearLeftWheelDirectionToForward();
	}

	void CarSystem::setRearRightWheelDirectionToForward()
	{
		this->movement_system->setRearRightWheelDirectionToForward();
	}

	void CarSystem::setRearWheelsDirectionToBackward()
	{
		this->movement_system->setRearWheelsDirectionToBackward();
	}

	void CarSystem::setRearLeftWheelDirectionToBackward()
	{
		this->movement_system->setRearLeftWheelDirectionToBackward();
	}

	void CarSystem::setRearRightWheelDirectionToBackward()
	{
		this->movement_system->setRearRightWheelDirectionToBackward();
	}
}