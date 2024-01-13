#ifndef BEHAVIOUR_TREE_NODE_HPP
#define BEHAVIOUR_TREE_NODE_HPP

#pragma once

#include <string>
#include <memory>

#include "Status.hpp"

namespace behaviour_tree::node
{
	class Node
	{
	public:
		Node(std::string name) : name(name)
		{
		}

		virtual const Status run() = 0;

	protected:
		std::string name;
	};
}

#endif