#ifndef BEHAVIOUR_TREE_DECORATOR_HPP
#define BEHAVIOUR_TREE_DECORATOR_HPP

#pragma once

#include "../Node.hpp"

#include "DecoratorType.hpp"

namespace behaviour_tree::node::decorator
{
	class Decorator : public Node
	{
	public:
		Decorator(std::string name, std::unique_ptr<Node> child) : Node(name), child(std::move(child))
		{
		}

		virtual const DecoratorType type() const = 0;

	protected:
		std::unique_ptr<Node> child;
	};
}

#endif