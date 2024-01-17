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
		Status toRoot(const std::string& id);
		void log(std::string message);

	private:
		std::vector<std::string> messages;
		std::shared_ptr<BehaviourTree> behaviour_tree;
	};
}

#endif
