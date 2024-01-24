#ifndef BEHAVIOUR_TREE_CUSTOM_NODE_HPP
#define BEHAVIOUR_TREE_CUSTOM_NODE_HPP

#pragma once

#include "../Node.hpp"

namespace behaviour_tree::node::custom
{
	class CustomNode : public Node
	{
	public:
		CustomNode(const std::string& name) : Node(name) {

		}
	};
}

#endif