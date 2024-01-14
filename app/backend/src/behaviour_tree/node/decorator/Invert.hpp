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
			case Status::Success:
				return Status::Failure;
			case Status::Failure:
				return Status::Success;
			}
		}

		const std::string toString() const override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Invert name="{}">{}</Invert>)", name, this->child->toString());
			else
				return fmt::format(R"(<Invert>{}</Invert>)", this->child->toString());
		}
	};
}

#endif