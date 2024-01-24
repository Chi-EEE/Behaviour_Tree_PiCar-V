#ifndef BEHAVIOUR_TREE_NODE_HPP
#define BEHAVIOUR_TREE_NODE_HPP

#pragma once

#include <string>
#include <memory>

#include <fmt/format.h>

#include "../Status.hpp"
#include "../Context.h"

namespace behaviour_tree::node
{
	class Node
	{
	public:
		Node(const std::string& name) : name(name)
		{
		}

		virtual const Status tick(const int& tick_count, Context& context) = 0;

		virtual const std::string toString() const = 0;

		const std::string& getName() const
		{
			return this->name;
		}

	protected:
		const std::string name;
	};
}

#endif