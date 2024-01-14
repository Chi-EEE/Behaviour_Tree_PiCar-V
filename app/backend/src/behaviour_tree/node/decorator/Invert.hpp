#ifndef BEHAVIOUR_TREE_INVERT_HPP
#define BEHAVIOUR_TREE_INVERT_HPP

#pragma once

#include "Decorator.hpp"

namespace behaviour_tree::node::decorator
{
	class Invert : public Decorator
	{
	public:
		Invert(const std::string& name, std::unique_ptr<Node> child) : Decorator(name, std::move(child)) {}

		const DecoratorType type() const override { return DecoratorType::Invert; }

		const Status run() override
		{
			auto status = this->child->run();
			switch (status)
			{
			case Status::SUCCESS:
				return Status::FAILURE;
			case Status::FAILURE:
				return Status::SUCCESS;
			}
		}
	};
}

#endif