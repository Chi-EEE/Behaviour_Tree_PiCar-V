#ifndef BEHAVIOUR_TREE_CONDITION_HPP
#define BEHAVIOUR_TREE_CONDITION_HPP

#pragma once

#include "TaskNode.hpp"

namespace behaviour_tree::node::task
{
	class Condition : public TaskNode
	{
	public:
		Condition(const std::string& name) : TaskNode(name) {

		}
	};
}

#endif