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
	class Context : public std::enable_shared_from_this<Context>
	{
	public:
		Context(std::shared_ptr<BehaviourTree> behaviour_tree);
		std::shared_ptr<BehaviourTree> getBehaviourTree() const { return this->behaviour_tree; };
		Status UseRoot(const int& tick_count, const std::string& id);

		virtual void _() = 0;
	private:
		std::shared_ptr<BehaviourTree> behaviour_tree;
	};
}

#endif
