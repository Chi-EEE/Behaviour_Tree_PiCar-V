#include "Context.h"
#include "BehaviourTree.hpp"

namespace behaviour_tree {
	Context::Context(std::shared_ptr<BehaviourTree> behaviour_tree) : behaviour_tree(behaviour_tree)
	{
	}

	void Context::update(const int& tick_count)
	{
		if (this->node_trace.size() == 0) {
			this->behaviour_tree->tick(tick_count, shared_from_this());
		}
		else {
			for (auto& node_trace : this->node_trace) {
				node_trace.first->tick(tick_count, shared_from_this());
			}
		}
	}

	Status Context::UseRoot(const int& tick_count, const std::string& id) {
		return this->behaviour_tree->UseRoot(tick_count, shared_from_this(), id);
	}

	void Context::pushNode(std::pair<std::shared_ptr<node::Node>, int> node_trace)
	{
		this->node_trace.push_back(node_trace);
	}
	
	void Context::popNode()
	{
		this->node_trace.pop_back();
	}
}