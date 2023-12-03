#include "Car.h"

Car::Car(const std::string& websocket_url, std::unique_ptr<LidarStrategy> lidar) : lidar(std::move(lidar))
{
    this->initalize(websocket_url);
}

Car::~Car()
{
    this->terminate();
}

void Car::run()
{
	this->web_socket.start();
	{
		json first_message = { {"type", "car"} };
		this->web_socket.send(first_message.dump());
	}
	this->lidar->start();
	while (true)
    {
		json output = json::array();
		std::vector<Measure> scan = this->lidar->scan();
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

void Car::initalize(const std::string &websocket_url)
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

void Car::terminate()
{
    ix::uninitNetSystem();
}
