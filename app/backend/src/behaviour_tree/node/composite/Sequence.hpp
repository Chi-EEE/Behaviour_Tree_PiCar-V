#ifndef BEHAVIOUR_TREE_SEQUENCE_HPP
#define BEHAVIOUR_TREE_SEQUENCE_HPP

#pragma once

#include "Composite.hpp"

namespace behaviour_tree::node::composite
{
	class Sequence : public Composite
	{
	public:
		Sequence(std::string name, std::vector<std::unique_ptr<Node>> children) : Composite(name, std::move(children)) {}

		const CompositeType type() const override { return CompositeType::Sequence; }

		const Status run() override
		{
			for (auto &child : this->children)
			{
				auto status = child->run();
				if (status == Status::FAILURE)
				{
					return Status::FAILURE;
				}
			}
			return Status::SUCCESS;
		}
	};
}

#endif