#ifndef BEHAVIOUR_TREE_NODE_HPP
#define BEHAVIOUR_TREE_NODE_HPP

#pragma once

#include <string>
#include <memory>

#include "../Context.hpp"
#include "../Status.hpp"

namespace behaviour_tree::node
{
	class Node
	{
	public:
		Node(const std::string& name) : name(name)
		{
		}

		virtual const Status run(Context& context) = 0;

		virtual const std::string toString() const = 0;

		const std::string& getName() const
		{
			return name;
		}

	protected:
		const std::string name;
	};
}

#endif