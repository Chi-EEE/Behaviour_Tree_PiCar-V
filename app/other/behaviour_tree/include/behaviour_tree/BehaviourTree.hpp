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

		void run(Context& context)
		{
			for (auto& root : this->roots)
			{
				if (root->getId() == "Main")
				{
					root->run(context);
					break;
				}
			}
		}

		Status toRoot(Context& context, const std::string& id) {
			for (auto& root : this->roots)
			{
				if (root->getId() == id)
				{
					return root->run(context);
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
			return out;
		}

	private:
		std::vector<std::shared_ptr<Root>> roots;
	};
}

#endif