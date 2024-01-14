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

		const Status run() override
		{
			for (auto &child : this->children)
			{
				auto status = child->run();
				if (status == Status::SUCCESS)
				{
					return Status::SUCCESS;
				}
			}
			return Status::FAILURE;
		}

		const std::string toString() const override {
			std::string out;
			for (auto& child : this->children)
			{
				out += child->toString();
			}
			return fmt::format(R"(<Selector name="{}">{}</Selector>)", this->getName(), out);
		}
	};
}

#endif