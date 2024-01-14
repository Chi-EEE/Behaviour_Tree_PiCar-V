#ifndef BEHAVIOUR_TREE_REPEAT_HPP
#define BEHAVIOUR_TREE_REPEAT_HPP

#pragma once

#include "Decorator.hpp"

namespace behaviour_tree::node::decorator
{
	class Repeat : public Decorator
	{
	public:
		Repeat(const std::string& name, std::unique_ptr<Node> child, const int& count, const bool& break_on_fail) : Decorator(name, std::move(child)), count(count), break_on_fail(break_on_fail)
		{
		}

		const DecoratorType type() const override { return DecoratorType::Repeat; }

		const Status run() override
		{
			for (int i = 0; i < this->count; i++)
			{
				auto status = this->child->run();
				switch (status)
				{
				case Status::SUCCESS:
					continue;
				case Status::FAILURE:
					if (this->break_on_fail)
					{
						return Status::FAILURE;
					}
					continue;
				}
			}
		}

		const std::string toString() const override {
			return fmt::format(R"(<Repeat name="{}" count="{}" break_on_fail="{}">{}</Repeat>)", this->getName(), this->getCount(), this->getBreakOnFail() ? "true" : "false", this->child->toString());
		}

		const int& getCount() const {
			return this->count;
		}

		const bool& getBreakOnFail() const {
			return this->break_on_fail;
		}

	private:
		const int count;
		const bool break_on_fail;
	};
}

#endif