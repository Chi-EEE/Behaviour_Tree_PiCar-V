#ifndef BEHAVIOUR_TREE_REPEAT_HPP
#define BEHAVIOUR_TREE_REPEAT_HPP

#pragma once

#include <numeric>

#include "Decorator.hpp"

namespace behaviour_tree::node::decorator
{
	class Repeat final : public Decorator
	{
	public:
		Repeat(const std::string& name, std::shared_ptr<Node> child, const unsigned long& count, const bool& break_on_fail) : Decorator(name, std::move(child)), count(count), break_on_fail(break_on_fail)
		{
		}

		const DecoratorType type() const final override { return DecoratorType::Repeat; }

		void start(std::shared_ptr<Context> context) final override {
			context->pushNodeTrace(std::make_pair(shared_from_this(), 0));
		}

		void finish(std::shared_ptr<Context> context) final override {
			context->popNode();
		}

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override
		{
			for (int i = 0; i < this->count; i++)
			{
				auto status = this->child->tick(tick_count, context);
				switch (status)
				{
				case Status::Success:
					continue;
				case Status::Failure:
					if (this->break_on_fail)
					{
						return Status::Failure;
					}
					continue;
				}
			}
		}

		const std::string toString() const final override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Repeat name='{}' count='{}' break_on_fail='{}'>{}</Repeat>)", name, this->getCount(), this->getBreakOnFail() ? "true" : "false", this->child->toString());
			else
				return fmt::format(R"(<Repeat count='{}' break_on_fail='{}'>{}</Repeat>)", this->getCount(), this->getBreakOnFail() ? "true" : "false", this->child->toString());
		}

		const unsigned long& getCount() const {
			return this->count;
		}

		const bool& getBreakOnFail() const {
			return this->break_on_fail;
		}

	private:
		const unsigned long count;
		const bool break_on_fail;
	};
}

#endif