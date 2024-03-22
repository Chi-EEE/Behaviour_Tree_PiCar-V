#ifndef BEHAVIOUR_TREE_SEQUENCE_HPP
#define BEHAVIOUR_TREE_SEQUENCE_HPP

#pragma once

#include "Composite.hpp"

namespace behaviour_tree::node::composite
{
	class Sequence final : public Composite
	{
	public:
		Sequence(const std::string& name, std::vector<std::shared_ptr<Node>> children) : Composite(name, std::move(children)) {}

		const CompositeType type() const final override { return CompositeType::Sequence; }

		void start(std::shared_ptr<Context> context) final override {
			this->previous_start_index = -1;
		}

		const Status run(const int tick_count, std::shared_ptr<Context> context) final override
		{
			return this->run(tick_count, context, 0);
		}

		const Status run(const int tick_count, std::shared_ptr<Context> context, const int start_index) final override
		{
			for (int i = start_index; i < this->children.size(); i++)
			{
				if (this->previous_start_index != i) {
					context->pushNodeTrace(std::make_pair(shared_from_this(), i));
				}
				auto& child = this->children[i];
				auto status = child->tick(tick_count, context);
				switch (status) {
				case Status::Running:
					this->previous_start_index = i;
					return Status::Running;
				case Status::Success:
					context->popNode();
					continue;
				case Status::Failure:
					context->popNode();
					return Status::Failure;
				}
			}
			return Status::Success;
		}

		const std::string toString() const final override {
			std::string out;
			for (auto& child : this->children)
			{
				out += child->toString();
			}
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Sequence name='{}'>{}</Sequence>)", name, out);
			else
				return fmt::format(R"(<Sequence>{}</Sequence>)", out);
		}

	private:
		int previous_start_index = -1;
	};
}

#endif