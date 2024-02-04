#include "Context.h"
#include "BehaviourTree.hpp"
#include "node/Node.hpp"
#include "node/composite/Composite.hpp"

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
			for (auto node_trace = this->node_trace.begin(); node_trace != this->node_trace.end(); ) {
				auto& [node, index] = *node_trace;
				if (composite::Composite* composite = dynamic_cast<composite::Composite*>(node.get()))
				{
					auto status = composite->tick(tick_count, shared_from_this(), index);
					if (status == Status::Success || status == Status::Failure) {
						node_trace = this->node_trace.erase(node_trace);
					}
					else {
						break;
					}
				}
				else {
					auto status = node->tick(tick_count, shared_from_this());
					if (status == Status::Success || status == Status::Failure) {
						node_trace = this->node_trace.erase(node_trace);
					}
					else {
						break;
					}
				}
			}
		}
	}

	Status Context::UseRoot(const int& tick_count, const std::string& id) {
		return this->behaviour_tree->UseRoot(tick_count, shared_from_this(), id);
	}

	void Context::pushNodeTrace(std::pair<std::shared_ptr<node::Node>, int> node_trace)
	{
		this->node_trace.push_back(node_trace);
	}

	void Context::popNode()
	{
		this->node_trace.pop_back();
	}
}