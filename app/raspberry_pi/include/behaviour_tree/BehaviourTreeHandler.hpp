#ifndef BEHAVIOURTREEHANDLER_HPP
#define BEHAVIOURTREEHANDLER_HPP

#pragma once

#include <vector>
#include <tsl/robin_map.h>

#include <nod/nod.hpp>

#include "behaviour_tree/BehaviourTreeParser.hpp"
#include "behaviour_tree/node/custom/CarCustomNodeParser.hpp"
#include "CarContext.hpp"

namespace behaviour_tree
{
	class BehaviourTreeHandler
	{
	public:
		BehaviourTreeHandler(std::shared_ptr<car::system::CarSystem> car_system, nod::signal<void(std::string, std::string)>& custom_command_signal, bool autorun) : car_system(car_system), autorun(autorun)
		{
			// The BehaviourTreeParser does not come with a CustomNodeParser since each program can have a different set of Action nodes
			BehaviourTreeParser::instance().setCustomNodeParser(std::make_unique<node::custom::CarCustomNodeParser>(CarCustomNodeParser()));
			custom_command_signal.connect([&](std::string custom_command_type, std::string custom)
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
					if (autorun) {
						auto context = CarContext(behaviour_tree, car_system);
						behaviour_tree->run(context);
					}
					else {
						this->addBehaviourTree(behaviour_tree);
					}
				}
			);
		}

		void addBehaviourTree(std::shared_ptr<BehaviourTree> behaviour_tree) {
			this->behaviour_trees.insert({ this->id, behaviour_tree });
			this->id++;
		}

		void removeBehaviourTree(int id) {
			this->behaviour_trees.erase(id);
		}

		std::shared_ptr<BehaviourTree> getBehaviourTree(int id) {
			return this->behaviour_trees.at(id);
		}

	private:
		std::shared_ptr<car::system::CarSystem> car_system;
		bool autorun = false;

		int id = 0;
		tsl::robin_map<int, std::shared_ptr<BehaviourTree>> behaviour_trees;
	};
} // namespace behaviour_tree

#endif