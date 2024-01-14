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

		const Status run() override
		{
			// TODO:
			return Status::SUCCESS;
		}

		const std::string toString() const override {
			return fmt::format(R"(<Wait name="{}" ms="{}">{}</Wait>)", this->getName(), this->getMS(), this->child->toString());
		}

		const int& getMS() const {
			return this->ms;
		}

	private:
		const int ms;
	};
}

#endif