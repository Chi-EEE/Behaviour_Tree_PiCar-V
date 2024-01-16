#ifndef BEHAVIOUR_TREE_WAIT_HPP
#define BEHAVIOUR_TREE_WAIT_HPP

#pragma once

#include "Decorator.hpp"

namespace behaviour_tree::node::decorator
{
	class Wait : public Decorator
	{
	public:
		Wait(const std::string& name, std::unique_ptr<Node> child, const int& ms) : Decorator(name, std::move(child)), ms(ms)
		{
		}

		const DecoratorType type() const override { return DecoratorType::Repeat; }

		const Status run(Context& context) override
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(this->ms));
			return Status::Success;
		}

		const std::string toString() const override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Wait name="{}" ms="{}">{}</Wait>)", name, this->getMS(), this->child->toString());
			else
				return fmt::format(R"(<Wait ms="{}">{}</Wait>)", this->getMS(), this->child->toString());
		}

		const int& getMS() const {
			return this->ms;
		}

	private:
		const int ms;
	};
}

#endif