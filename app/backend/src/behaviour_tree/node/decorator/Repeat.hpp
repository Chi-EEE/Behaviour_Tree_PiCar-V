#ifndef BEHAVIOUR_TREE_REPEAT_HPP
#define BEHAVIOUR_TREE_REPEAT_HPP

#pragma once

#include "Decorator.hpp"

namespace behaviour_tree::node::decorator
{
	class Repeat : public Decorator
	{
	public:
		Repeat(std::string name, std::unique_ptr<Node> child, int count, bool break_on_fail) : Decorator(name, std::move(child)), count(count), break_on_fail(break_on_fail)
		{
		}

		const DecoratorType type() const override { return DecoratorType::Repeat; }

		const Status run() override
		{
			for (int i = 0; i < this->count; i++)
			{
				auto status = this->child->run();
				switch (status)
				{
				case Status::SUCCESS:
					continue;
				case Status::FAILURE:
					if (this->break_on_fail)
					{
						return Status::FAILURE;
					}
					continue;
				}
			}
		}

	private:
		int count;
		bool break_on_fail;
	};
}

#endif