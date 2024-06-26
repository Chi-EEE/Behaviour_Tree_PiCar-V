#ifndef BEHAVIOURTREEHANDLER_HPP
#define BEHAVIOURTREEHANDLER_HPP

#pragma once

#include <string>
#include <vector>

#include <nod/nod.hpp>

#include "utils/Utility.hpp"

#include "car/plugin/Plugin.h"

#include "behaviour_tree/BehaviourTreeParser.hpp"
#include "behaviour_tree/node/custom/CarCustomNodeParser.hpp"

#include "CarContext.hpp"

namespace behaviour_tree
{
	class BehaviourTreeHandler : public car::plugin::Plugin
	{
	public:
		void initialize(std::shared_ptr<car::system::CarSystem> car_system) final override
		{
			this->car_system = car_system;
			// The BehaviourTreeParser does not come with a CustomNodeParser since each program can have a different set of Action nodes
			BehaviourTreeParser::instance().setCustomNodeParser(std::make_shared<node::custom::CarCustomNodeParser>(CarCustomNodeParser()));
			this->car_system->getMessagingSystem()->getCommandSignal().connect(std::bind(&BehaviourTreeHandler::handleCommand, this, std::placeholders::_1, std::placeholders::_2));
		}

		void handleCommand(const std::string message, const rapidjson::Document &message_json)
		{
			const std::string command = message_json["command"].GetString();
			if (command != "behaviour_tree")
			{
				spdlog::error(R"(The property "command" does not match "behaviour_tree", {})", command);
				return;
			}
			if (!message_json.HasMember("action") || !message_json["action"].IsString())
			{
				spdlog::error(R"(The property "action" does not exist in the given json.)");
				return;
			}
			const std::string action = message_json["action"].GetString();
			switch (utils::hash(action))
			{
			case utils::hash("set"):
			{
				this->setBehaviourTree(message_json);
				break;
			}
			case utils::hash("start"):
			{
				this->startBehaviourTree();
				break;
			}
			case utils::hash("stop"):
			{
				this->stopBehaviourTree();
				break;
			}
			default:
				spdlog::error(R"(The property "action" does not match "set" or "start", {})", action);
				break;
			};
		}

		void setBehaviourTree(const rapidjson::Document &message_json)
		{
			if (!message_json.HasMember("data") || !message_json["data"].IsString())
			{
				spdlog::error(R"(The property "data" does not exist in the given json.)");
				return;
			}
			try
			{
				auto maybe_behaviour_tree = BehaviourTreeParser::instance().parseXML(message_json["data"].GetString());
				if (!maybe_behaviour_tree.has_value())
				{
					spdlog::error(R"(Unable to parse the given behaviour tree | {})", maybe_behaviour_tree.error());
					return;
				}
				auto &behaviour_tree = maybe_behaviour_tree.value();
				spdlog::info("Behaviour tree parsed successfully | {}", behaviour_tree->toString());
				this->_setBehaviourTree(behaviour_tree);
			}
			catch (std::exception &e)
			{
				spdlog::error("An error has occurred while parsing the given behaviour tree: {}", e.what());
			}
		}

		void startBehaviourTree()
		{
			assert(this->car_system != nullptr);
			if (this->behaviour_tree == nullptr)
			{
				spdlog::error("The Behaviour tree has not been set");
				return;
			}
			this->behaviour_tree->resetCycles();
			this->tick_count = 0;
			std::shared_ptr<Context> context = std::make_shared<CarContext>(this->behaviour_tree, this->car_system);
			this->context = context;
			spdlog::info("Starting the given Behaviour tree");
		}

		void stopBehaviourTree()
		{
			assert(this->car_system != nullptr);
			this->context = nullptr;
			spdlog::info("Stopped any Behaviour Tree context");
		}

		void update() final override
		{
			if (this->context == nullptr)
			{
				return;
			}
			if (this->context->canRun())
			{
                const std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
                // TODO:
                if (now - this->last_connected >= this->car_system->getConfiguration()->behaviour_tree_update_ms_interval) {
                    this->context->update(this->tick_count);
                    this->tick_count++;
                    this->last_connected = now;
                }
			}
			else
			{
				this->context = nullptr;
			}
		}

		void stop() final override
		{
			this->context = nullptr;
		}

		std::string getName() final override
		{
			return "BehaviourTreeHandler";
		}

		void _setBehaviourTree(std::shared_ptr<BehaviourTree> behaviour_tree)
		{
			this->behaviour_tree = behaviour_tree;
		}

	private:
		std::shared_ptr<car::system::CarSystem> car_system;
		
        std::shared_ptr<BehaviourTree> behaviour_tree;
		std::shared_ptr<Context> context;

		int tick_count = 0;
        
        // This is initialized as 0
        std::chrono::time_point<std::chrono::steady_clock> last_connected;
	};
} // namespace behaviour_tree

#endif