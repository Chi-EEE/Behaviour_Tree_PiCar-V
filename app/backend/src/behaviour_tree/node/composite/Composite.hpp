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
		Composite(std::string name, std::vector<std::shared_ptr<Node>> children) : Node(name), children(children)
		{
		}
		virtual const CompositeType type() const = 0;

	protected:
		std::vector<std::shared_ptr<Node>> children;
	};
}

#endif