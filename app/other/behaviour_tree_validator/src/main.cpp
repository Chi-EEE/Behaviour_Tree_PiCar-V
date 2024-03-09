#include <iostream>

#include <cxxopts.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "behaviour_tree/BehaviourTreeParser.hpp"
#include "behaviour_tree/node/custom/CarCustomNodeParser.hpp"

using namespace behaviour_tree;

int main(int argc, char* argv[])
{
	BehaviourTreeParser::instance().setCustomNodeParser(std::make_shared<node::custom::CarCustomNodeParser>(CarCustomNodeParser()));
	
	cxxopts::Options options("Behaviour Tree Validator", "Program to validate Behaviour Tree");

	options.add_options()
		("behaviour_tree", "Behaviour Tree XML to validate", cxxopts::value<std::string>());

	auto cli_result = options.parse(argc, argv);

	std::string behaviour_tree = cli_result["behaviour_tree"].as<std::string>();

    auto behaviour_tree_result = BehaviourTreeParser::instance().parseXML(behaviour_tree);

    rapidjson::Document json_output_doc;
    json_output_doc.SetObject();

    if (behaviour_tree_result.has_value()) {
        json_output_doc.AddMember("success", true, json_output_doc.GetAllocator());
        json_output_doc.AddMember("result", rapidjson::Value(behaviour_tree_result.value()->toString().c_str(), json_output_doc.GetAllocator()), json_output_doc.GetAllocator());
    }
    else {
        json_output_doc.AddMember("success", false, json_output_doc.GetAllocator());
        json_output_doc.AddMember("message", rapidjson::Value(behaviour_tree_result.error().c_str(), json_output_doc.GetAllocator()), json_output_doc.GetAllocator());
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json_output_doc.Accept(writer);

    std::cout << buffer.GetString() << std::endl;

	return 0;
}
