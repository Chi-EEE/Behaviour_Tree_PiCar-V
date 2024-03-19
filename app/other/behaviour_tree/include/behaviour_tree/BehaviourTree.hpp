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
		BehaviourTree(unsigned int cycle_limit, std::vector<std::shared_ptr<Root>> roots) : cycle_limit(cycle_limit), roots(std::move(roots))
		{
		}

		void start()
		{
			for (auto& root : this->roots)
			{
				if (root->getId() == "Main")
				{
					this->root_to_use = root;
					return;
				}
			}
			this->root_to_use = this->roots[0];
		}

		Status tick(const int& tick_count, std::shared_ptr<Context> context)
		{
			const Status status = this->root_to_use->tick(tick_count, context);
			if (status != Status::Running)
			{
				this->cycles++;
			}
			return status;
		}

		Status UseRoot(const int& tick_count, std::shared_ptr<Context> context, const std::string& id) {
			for (auto& root : this->roots)
			{
				if (root->getId() == id)
				{
					return root->tick(tick_count, context);
				}
			}
			return Status::Failure;
		}

		bool canRun() const {
			if (this->cycle_limit <= 0)
			{
				return true;
			}
			return this->cycles < this->cycle_limit;
		}

		void incrementCycle() {
			this->cycles++;
		}

		const std::string toString() const {
			std::string out;
			for (auto& root : this->roots)
			{
				out += root->toString();
			}
			return fmt::format(R"(<BehaviourTree cycle_limit='{}'>{}</BehaviourTree>)", this->cycle_limit, out);
		}

	private:
		int cycles = 0;
		std::shared_ptr<Root> root_to_use;

		const std::vector<std::shared_ptr<Root>> roots;
		const unsigned int cycle_limit;
	};
}

#endif