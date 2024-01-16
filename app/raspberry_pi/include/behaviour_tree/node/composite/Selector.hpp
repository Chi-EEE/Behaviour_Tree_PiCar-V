#ifndef BEHAVIOUR_TREE_SELECTOR_HPP
#define BEHAVIOUR_TREE_SELECTOR_HPP

#pragma once

#include "Composite.hpp"

namespace behaviour_tree::node::composite
{
	class Selector : public Composite
	{
	public:
		Selector(const std::string& name, std::vector<std::unique_ptr<Node>> children) : Composite(name, std::move(children)) {}

		const CompositeType type() const override { return CompositeType::Selector; }

		const Status run(Context& context) override
		{
			for (auto& child : this->children)
			{
				auto status = child->run(context);
				if (status == Status::Success)
				{
					return Status::Success;
				}
			}
			return Status::Failure;
		}

		const std::string toString() const override {
			std::string out;
			for (auto& child : this->children)
			{
				out += child->toString();
			}
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Selector name="{}">{}</Selector>)", name, out);
			else
				return fmt::format(R"(<Selector>{}</Selector>)", out);
		}
	};
}

#endif