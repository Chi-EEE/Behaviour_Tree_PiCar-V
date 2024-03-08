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
		BehaviourTree(bool repeat, std::vector<std::shared_ptr<Root>> roots) : repeat(repeat), roots(std::move(roots))
		{
		}

		void start(std::shared_ptr<Context> context)
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
			if (this->repeat)
			{
				return true;
			}
			return this->cycles < 1;
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
		int cycles = 0;
		std::shared_ptr<Root> root_to_use;

		const std::vector<std::shared_ptr<Root>> roots;
		const bool repeat;
	};
}

#endif