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
		Repeat(const std::string& name, std::shared_ptr<Node> child, const unsigned long& amount, const bool& break_on_fail) : Decorator(name, std::move(child)), amount(amount), break_on_fail(break_on_fail)
		{
		}

		const DecoratorType type() const final override { return DecoratorType::Repeat; }

		void start(std::shared_ptr<Context> context) final override {
			this->count = 0;
		}

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override
		{
			auto status = this->child->tick(tick_count, context);
			switch (status)
			{
			case Status::Success:
				this->count++;
				if (this->count >= this->amount)
				{
					return Status::Success;
				}
				return Status::Running;
			case Status::Running:
				return Status::Running;
			case Status::Failure:
				if (this->break_on_fail)
					return Status::Failure;
				return Status::Running;
			}
		}

		const std::string toString() const final override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Repeat name='{}' amount='{}' break_on_fail='{}'>{}</Repeat>)", name, this->getAmount(), this->getBreakOnFail() ? "true" : "false", this->child->toString());
			else
				return fmt::format(R"(<Repeat amount='{}' break_on_fail='{}'>{}</Repeat>)", this->getAmount(), this->getBreakOnFail() ? "true" : "false", this->child->toString());
		}

		const unsigned long& getAmount() const {
			return this->amount;
		}

		const bool& getBreakOnFail() const {
			return this->break_on_fail;
		}

		const unsigned long& getCount() const {
			return this->count;
		}

	private:
		const unsigned long amount;
		const bool break_on_fail;
	
		unsigned long count = 0;
	};
}

#endif