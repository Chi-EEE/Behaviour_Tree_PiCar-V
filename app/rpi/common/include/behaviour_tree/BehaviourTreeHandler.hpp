#ifndef BEHAVIOURTREEHANDLER_HPP
#define BEHAVIOURTREEHANDLER_HPP

#pragma once

#include <string>
#include <vector>

#include <nod/nod.hpp>

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
			this->car_system->getCustomCommandSignal().connect([&](std::string custom_command_type, std::string custom)
				{
					if (custom_command_type != "behaviour_tree") {
						return;
					}
					auto maybe_behaviour_tree = BehaviourTreeParser::instance().parseXML(custom);
					if (!maybe_behaviour_tree.has_value()) {
						spdlog::error("Behaviour tree parsing failed | {} | {}", maybe_behaviour_tree.error(), custom);
						return;
					}
					auto& behaviour_tree = maybe_behaviour_tree.value();
					spdlog::info("Behaviour tree parsed successfully | {}", behaviour_tree->toString());
					this->setBehaviourTree(behaviour_tree); });
		}

		void update() final override
		{
			if (this->context != nullptr)
			{
				this->context->update(this->tick_count);
				++this->tick_count;
			}
		}

		void start()
		{
			this->tick_count = 0;
			std::shared_ptr<Context> context = std::make_shared<CarContext>(CarContext(this->behaviour_tree, this->car_system));
			this->context = context;
			this->behaviour_tree->start(context);
		}

		void stop() final override
		{
			this->context = nullptr;
		}

		std::string getName() final override
		{
			return "BehaviourTreeHandler";
		}

		void setBehaviourTree(std::shared_ptr<BehaviourTree> behaviour_tree) {
			this->behaviour_tree = behaviour_tree;
		}

	private:
		std::shared_ptr<BehaviourTree> behaviour_tree;
		
		std::shared_ptr<Context> context;

		int tick_count = 0;

		std::shared_ptr<car::system::CarSystem> car_system;
	};
} // namespace behaviour_tree

#endif