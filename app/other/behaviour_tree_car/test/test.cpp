#include <iostream>
#include <chrono>
#include <filesystem>
#include <thread>
#include <memory>

#include <cxxopts.hpp>

#include "behaviour_tree/BehaviourTreeParser.hpp"
#include "behaviour_tree/node/custom/CarCustomNodeParser.hpp"

#include "behaviour_tree/BehaviourTreeHandler.hpp"

using namespace car::system;
using namespace car::system::lidar;
using namespace car::system::movement::controller;

using namespace car::configuration;
using namespace car::plugin;

using namespace behaviour_tree;


int main(int argc, const char* argv[])
{
	BehaviourTreeParser::instance().setCustomNodeParser(std::make_shared<node::custom::CarCustomNodeParser>(CarCustomNodeParser()));

	std::string ip_address = "";
	int port = 0;
	std::string rpi_name = "";
	std::string room_name = "";

	std::shared_ptr<Configuration> configuration = std::make_shared<Configuration>(Configuration{
		ip_address,
		port,
		rpi_name,
		room_name,
		});

	cxxopts::Options options("Behaviour Tree Tester", "Program to test Behaviour Trees");

	options.add_options()
		("test_file", "Behaviour Tree Test File", cxxopts::value<std::string>());

	auto cli_result = options.parse(argc, argv);

	std::string test_file_string = cli_result["test_file"].as<std::string>();
	
	auto behaviour_tree_result = BehaviourTreeParser::instance().parseFileXML(test_file_string);

	if (!behaviour_tree_result.has_value())
	{
		spdlog::error("Unable to parse the Behaviour Tree: {}", behaviour_tree_result.error());
		return EXIT_FAILURE;
	}

	std::shared_ptr<BehaviourTree> behaviour_tree = behaviour_tree_result.value();

	std::shared_ptr<BehaviourTreeHandler> behaviour_tree_handler = std::make_shared<BehaviourTreeHandler>(BehaviourTreeHandler());
	behaviour_tree_handler->setBehaviourTree(behaviour_tree);

	std::unique_ptr<PluginManager> plugin_manager = std::make_unique<PluginManager>();
	plugin_manager->addPlugin(behaviour_tree_handler);

	std::shared_ptr<CarSystem> car_system = std::make_shared<CarSystem>(
		configuration,
		std::move(scanner),
		std::move(messaging_system),
		std::move(movement_system),
		std::move(plugin_manager));

	car_system->initialize();

	behaviour_tree_handler->start();

	std::cout << "Press any key to exit the loop." << std::endl;
	while (!kbhit()) {
		car_system->update();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	car_system->terminate();

	return 0;
}
