#ifndef BEHAVIOUR_TREE_CONDITION_HPP
#define BEHAVIOUR_TREE_CONDITION_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

namespace behaviour_tree::node::custom
{
	class Condition : public CustomNode
	{
	public:
		Condition(const std::string& name) : CustomNode(name) {

		}
	};
}

#endif