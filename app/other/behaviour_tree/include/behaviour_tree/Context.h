#ifndef BEHAVIOUR_TREE_CONTEXT_HPP
#define BEHAVIOUR_TREE_CONTEXT_HPP

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Status.hpp"

namespace behaviour_tree
{
	class BehaviourTree;
}

namespace behaviour_tree
{
	class Context
	{
	public:
		Context(std::shared_ptr<BehaviourTree> behaviour_tree);
		Status UseRoot(const std::string& id);

	private:
		std::shared_ptr<BehaviourTree> behaviour_tree;
	};
}

#endif
