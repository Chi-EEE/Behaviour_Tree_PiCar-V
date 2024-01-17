#ifndef BEHAVIOUR_TREE_TASK_NODE_HPP
#define BEHAVIOUR_TREE_TASK_NODE_HPP

#pragma once

#include <fmt/format.h>

#include "../Context.h"
#include "../Status.hpp"

namespace behaviour_tree::task_node
{
	class TaskNode
	{
	public:
		virtual const Status run(Context& context) = 0;

		virtual const std::string toString() const = 0;
	};
}

#endif