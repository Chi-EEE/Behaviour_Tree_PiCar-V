#ifndef BEHAVIOUR_TREE_CONTEXT_HPP
#define BEHAVIOUR_TREE_CONTEXT_HPP

#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <variant>

#include "Status.hpp"

namespace behaviour_tree
{
	class BehaviourTree;
	namespace node
	{
		class Node;
		namespace composite
		{
			class Composite;
		}
	}
}

namespace behaviour_tree
{
	class Context : public std::enable_shared_from_this<Context>
	{
	public:
		Context(std::shared_ptr<BehaviourTree> behaviour_tree);
		void update(const int tick_count);
		Status UseRoot(const int tick_count, const std::string& id);

		std::shared_ptr<BehaviourTree> getBehaviourTree() const { return this->behaviour_tree; };

		void pushNodeTrace(std::pair<std::shared_ptr<node::Node>, int> node_trace);
		void popNode();

		bool canRun() const;

		// Necessary for the class to be a polymorphic
		virtual void _() {};
	private:
		std::shared_ptr<BehaviourTree> behaviour_tree;
		std::vector<std::pair<std::shared_ptr<node::Node>, int>> node_trace_list;
	};
}

#endif
