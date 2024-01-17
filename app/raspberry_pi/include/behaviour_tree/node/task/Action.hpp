#ifndef BEHAVIOUR_TREE_ACTION_HPP
#define BEHAVIOUR_TREE_ACTION_HPP

#pragma once

#include "TaskNode.hpp"

namespace behaviour_tree::node::task
{
	class Action : public TaskNode
	{
	public:
		Action(const std::string& name) : TaskNode(name) {

		}
	};
}

#endif