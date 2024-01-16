#ifndef BEHAVIOURTREEHANDLER_CXX
#define BEHAVIOURTREEHANDLER_CXX

#pragma once

#include <vector>
#include <tsl/robin_map.h>

#include <nod/nod.hpp>

#include "behaviour_tree/BehaviourTreeParser.hpp"
#include "CarActionParser.cxx"
#include "CarContext.cxx"

using namespace behaviour_tree;

namespace car::behaviour_tree
{
	class BehaviourTreeHandler
	{
	public:
		BehaviourTreeHandler(CarContext car_context, nod::signal<void(std::string, std::string)>& custom_command_signal, bool autorun) : car_context(car_context), autorun(autorun)
		{
			BehaviourTreeParser::instance().setActionParser(std::make_unique<CarActionParser>(CarActionParser()));
			custom_command_signal.connect([&](std::string custom_command_type, std::string custom)
				{
					if (custom_command_type != "behaviour_tree") {
						return;
					}
					auto maybe_behaviour_tree = BehaviourTreeParser::instance().parseXML(custom);
					if (maybe_behaviour_tree.has_value()) {
						return;
					}
					if (autorun) {
						std::shared_ptr<BehaviourTree>& behaviour_tree = std::move(maybe_behaviour_tree.value());
						behaviour_tree->run(car_context);
					}
					//this->addBehaviourTree(std::move(maybe_behaviour_tree.value()));
				}
			);
		}

		void addBehaviourTree(std::shared_ptr<BehaviourTree>& behaviour_tree) {
			this->behaviour_trees.insert({ this->id, std::move(behaviour_tree) });
			this->id++;
		}

		void removeBehaviourTree(int id) {
			this->behaviour_trees.erase(id);
		}

		std::shared_ptr<BehaviourTree> getBehaviourTree(int id) {
			return this->behaviour_trees.at(id);
		}

	private:
		CarContext car_context;
		bool autorun = false;

		int id = 0;
		tsl::robin_map<int, std::shared_ptr<BehaviourTree>> behaviour_trees;
	};
} // namespace behaviour_tree

#endif