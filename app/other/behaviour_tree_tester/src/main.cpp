#include <iostream>
#include <chrono>
#include <filesystem>
#include <thread>
#include <memory>

#include <cxxopts.hpp>

#include "behaviour_tree/BehaviourTreeParser.hpp"
#include "behaviour_tree/node/custom/CarCustomNodeParser.hpp"

using namespace behaviour_tree;

int main(int argc, const char* argv[])
{
	BehaviourTreeParser::instance().setCustomNodeParser(std::make_shared<node::custom::CarCustomNodeParser>(CarCustomNodeParser()));

	cxxopts::Options options("Behaviour Tree Tester", "Program to test Behaviour Trees (Does not use CarSystem)");

	options.add_options()
		("test_file", "Behaviour Tree Test File", cxxopts::value<std::string>());

	auto cli_result = options.parse(argc, argv);

	std::string test_file_path = cli_result["test_file"].as<std::string>();
	
	auto behaviour_tree_result = BehaviourTreeParser::instance().parseFileXML(test_file_path);

	if (!behaviour_tree_result.has_value())
	{
		std::cout << "Unable to parse the Behaviour Tree: " << behaviour_tree_result.error();
		return EXIT_FAILURE;
	}

	std::shared_ptr<BehaviourTree> behaviour_tree = behaviour_tree_result.value();

	std::shared_ptr<Context> context = std::make_shared<Context>(Context(behaviour_tree));
	
	int tick_count = 0;
	while (context->canRun()) {
		context->update(tick_count);
		tick_count++;
	}

	return 0;
}
