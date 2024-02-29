#ifndef BEHAVIOUR_TREE_SELECTOR_HPP
#define BEHAVIOUR_TREE_SELECTOR_HPP

#pragma once

#include "Composite.hpp"

namespace behaviour_tree::node::composite
{
	class Selector final : public Composite
	{
	public:
		Selector(const std::string& name, std::vector<std::shared_ptr<Node>> children) : Composite(name, std::move(children)) {}

		const CompositeType type() const final override { return CompositeType::Selector; }

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override
		{
			return this->run(tick_count, context, 0);
		}

		const Status run(const int& tick_count, std::shared_ptr<Context> context, const int& start_index) final override
		{
			for (int i = start_index; i < this->children.size(); i++)
			{
				auto& child = this->children[i];
				auto status = child->tick(tick_count, context);
				switch (status) {
				case Status::Failure:
					continue;
				case Status::Success:
					context->popNode();
					return Status::Success;
				case Status::Running:
					context->pushNodeTrace(std::make_pair(shared_from_this(), i));
					return Status::Running;
				}
			}
			return Status::Failure;
		}

		const std::string toString() const final override {
			std::string out;
			for (auto& child : this->children)
			{
				out += child->toString();
			}
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Selector name='{}'>{}</Selector>)", name, out);
			else
				return fmt::format(R"(<Selector>{}</Selector>)", out);
		}
	};
}

#endif