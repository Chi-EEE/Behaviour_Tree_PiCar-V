#ifndef BEHAVIOURTREEHANDLER_HPP
#define BEHAVIOURTREEHANDLER_HPP

#pragma once

#include <vector>
#include <tsl/robin_map.h>

#include <nod/nod.hpp>

#include "../../src/car/plugin/Plugin.cxx"

#include "behaviour_tree/BehaviourTreeParser.hpp"
#include "behaviour_tree/node/custom/CarCustomNodeParser.hpp"

#include "CarContext.hpp"

namespace behaviour_tree
{
	class BehaviourTreeHandler : public car::plugin::Plugin
	{
	public:
		void initialize(std::shared_ptr<car::system::CarSystem> car_system) override
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
					this->addBehaviourTree(behaviour_tree); });
		}

		void update() override
		{
			if (this->context != nullptr)
			{
				this->context->update(this->tick_count);
				++this->tick_count;
			}
		}

		void stop() override
		{
			this->context = nullptr;
		}

		void runBehaviourTree(std::shared_ptr<BehaviourTree> behaviour_tree)
		{
			this->tick_count = 0;
			std::shared_ptr<Context> context = std::make_shared<CarContext>(CarContext(behaviour_tree, this->car_system));
			this->context = context;
		}

		void addBehaviourTree(std::shared_ptr<BehaviourTree> behaviour_tree)
		{
			this->behaviour_trees.insert({this->id, behaviour_tree});
			this->id++;
		}

		void removeBehaviourTree(int id)
		{
			this->behaviour_trees.erase(id);
		}

		std::shared_ptr<BehaviourTree> getBehaviourTree(int id)
		{
			return this->behaviour_trees.contains(id) ? this->behaviour_trees[id] : nullptr;
		}

		const tsl::robin_map<int, std::shared_ptr<BehaviourTree>> &getBehaviourTrees() const
		{
			return this->behaviour_trees;
		}

	private:
		std::shared_ptr<Context> context;

		int tick_count = 0;

		std::shared_ptr<car::system::CarSystem> car_system;

		int id = 0;
		tsl::robin_map<int, std::shared_ptr<BehaviourTree>> behaviour_trees;
	};
} // namespace behaviour_tree

#endif