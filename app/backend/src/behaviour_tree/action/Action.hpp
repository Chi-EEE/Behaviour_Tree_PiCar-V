#ifndef BEHAVIOUR_TREE_ACTION_HPP
#define BEHAVIOUR_TREE_ACTION_HPP

#pragma once

#include "ActionType.hpp"

namespace behaviour_tree::action
{
	class Action
	{
	public:
		virtual ActionType type() const = 0;
	};
}

#endif