#ifndef BEHAVIOUR_TREE_COMPOSITE_HPP
#define BEHAVIOUR_TREE_COMPOSITE_HPP

#pragma once

#include <vector>

#include "../Node.hpp"
#include "CompositeType.hpp"

namespace behaviour_tree::node::composite
{
	class Composite : public Node
	{
	public:
		Composite(const std::string& name, std::vector<std::unique_ptr<Node>> children) : Node(name), children(std::move(children))
		{
		}
		virtual const CompositeType type() const = 0;

	protected:
		std::vector<std::unique_ptr<Node>> children;
	};
}

#endif