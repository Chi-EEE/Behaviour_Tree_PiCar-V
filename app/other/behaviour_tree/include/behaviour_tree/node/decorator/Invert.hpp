#ifndef BEHAVIOUR_TREE_INVERT_HPP
#define BEHAVIOUR_TREE_INVERT_HPP

#pragma once

#include "Decorator.hpp"

namespace behaviour_tree::node::decorator
{
	class Invert final : public Decorator
	{
	public:
		Invert(const std::string& name, std::shared_ptr<Node> child) : Decorator(name, std::move(child)) {}

		const DecoratorType type() const final override { return DecoratorType::Invert; }

		void start(std::shared_ptr<Context> context) final override {
			context->pushNodeTrace(std::make_pair(shared_from_this(), 0));
		}

		void finish(std::shared_ptr<Context> context) final override {
			context->popNode();
		}

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override
		{
			auto status = this->child->tick(tick_count, context);
			switch (status)
			{
			case Status::Success:
				return Status::Failure;
			case Status::Failure:
				return Status::Success;
			}
		}

		const std::string toString() const final override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Invert name="{}">{}</Invert>)", name, this->child->toString());
			else
				return fmt::format(R"(<Invert>{}</Invert>)", this->child->toString());
		}
	};
}

#endif