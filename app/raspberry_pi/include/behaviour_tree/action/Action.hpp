#ifndef BEHAVIOUR_TREE_ACTION_HPP
#define BEHAVIOUR_TREE_ACTION_HPP

#pragma once

#include "../Context.h"
#include "../Status.hpp"

namespace behaviour_tree::action
{
	class Action
	{
	public:
		virtual const Status run(Context& context) = 0;

		virtual const std::string toString() const = 0;
	};
}

#endif