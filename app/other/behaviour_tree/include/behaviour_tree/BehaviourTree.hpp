#ifndef BEHAVIOUR_TREE_HPP
#define BEHAVIOUR_TREE_HPP

#pragma once

/*
XML format inspired by: https://github.com/telcy/modular-behavior-tree & https://github.com/Defaultio/BehaviorTree3
*/

#include "Root.hpp"

namespace behaviour_tree
{
	class BehaviourTree
	{
	public:
		BehaviourTree(std::vector<std::shared_ptr<Root>> roots) : roots(std::move(roots))
		{
		}

		void tick(Context& context)
		{
			bool found = false;
			for (auto& root : this->roots)
			{
				if (root->getId() == "Main")
				{
					root->tick(context);
					found = true;
					break;
				}
			}
			if (!found) {
				this->roots[0]->tick(context);
			}
		}

		Status UseRoot(Context& context, const std::string& id) {
			for (auto& root : this->roots)
			{
				if (root->getId() == id)
				{
					return root->tick(context);
				}
			}
			return Status::Failure;
		}

		const std::string toString() const {
			std::string out;
			for (auto& root : this->roots)
			{
				out += root->toString();
			}
			return fmt::format(R"(<BehaviourTree>{}</BehaviourTree>)", out);
		}

	private:
		std::vector<std::shared_ptr<Root>> roots;
	};
}

#endif