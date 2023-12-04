#include "CarSystem.h"

namespace car_system {
	CarSystem::CarSystem(const std::string& websocket_url, std::unique_ptr<LidarDevice> lidar_device) : lidar_device(std::move(lidar_device))
	{
		this->initalize(websocket_url);
	}

	CarSystem::~CarSystem()
	{
		this->terminate();
	}

	void CarSystem::run()
	{
		spdlog::info("Running Car");
		this->web_socket.start();
		bool open = false;
		for (int i = 0; i < 3; i++) {
			if (this->web_socket.getReadyState() == ix::ReadyState::Open) {
				open = true;
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(i * 3000));
		}
		if (!open) {
			spdlog::error("Could not connect to websocket");
			return;
		}
		else
		{
			spdlog::info("Connected to websocket");
		}
		{
			json first_message = { {"type", "car"} };
			spdlog::info("Sending first message: {}", first_message.dump());
			this->web_socket.send(first_message.dump());
		}
		this->lidar_device->start();
		while (true)
		{
			json output = json::array();
			std::vector<Measure> scan = this->lidar_device->scan();
			for (const Measure& measure : scan)
			{
				output.push_back(
					{
					{"distance", measure.distance},
					{"angle", measure.angle},
					}
				);
			}
			web_socket.send(output.dump());
		}
	}

	void CarSystem::initalize(const std::string& websocket_url)
	{
		ix::initNetSystem();
		this->web_socket.setUrl(websocket_url);
		this->web_socket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
			{
				//if (msg->type == ix::WebSocketMessageType::Message)
				//{
				//	std::cout << "received message: " << msg->str << std::endl;
				//	std::cout << "> " << std::flush;
				//}
				//else if (msg->type == ix::WebSocketMessageType::Open)
				//{
				//	std::cout << "Connection established" << std::endl;
				//	std::cout << "> " << std::flush;
				//}
				//else if (msg->type == ix::WebSocketMessageType::Error)
				//{
				//	// Maybe SSL is not configured properly
				//	std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
				//	std::cout << "> " << std::flush;
				//}
				//else if (msg->type == ix::WebSocketMessageType::Close)
				//{
				//	std::cout << "Connection closed: " << msg->closeInfo.code << " " << msg->closeInfo.reason << std::endl;
				//	std::cout << "> " << std::flush;
				//}
			}
		);
	}

	void CarSystem::terminate()
	{
		this->lidar_device->terminate();
		this->web_socket.close();
		ix::uninitNetSystem();
	}

	void CarSystem::turn(float angle)
	{
	}

	void CarSystem::move(float distance)
	{
	}
}