#ifndef BEHAVIOUR_TREE_ACTION_HPP
#define BEHAVIOUR_TREE_ACTION_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

namespace behaviour_tree::node::custom
{
	class Action : public CustomNode
	{
	public:
		Action(const std::string& name) : CustomNode(name) {

		}
	};
}

#endif