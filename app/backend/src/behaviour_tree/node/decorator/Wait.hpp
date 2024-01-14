#ifndef BEHAVIOUR_TREE_WAIT_HPP
#define BEHAVIOUR_TREE_WAIT_HPP

#pragma once

#include "Decorator.hpp"

namespace behaviour_tree::node::decorator
{
	class Wait : public Decorator
	{
	public:
		Wait(std::string name, std::unique_ptr<Node> child, int ms) : Decorator(name, std::move(child)), ms(ms)
		{
		}

		const DecoratorType type() const override { return DecoratorType::Repeat; }

		const Status run() override
		{
			// TODO:
			return Status::SUCCESS;
		}

	private:
		int ms;
	};
}

#endif