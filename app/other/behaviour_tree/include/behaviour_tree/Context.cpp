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
		if (this->node_trace_list.size() == 0) {
			this->behaviour_tree->tick(tick_count, shared_from_this());
		}
		else {
			bool ignore = false;

			auto tick_node_trace = [this, &tick_count, &ignore](const auto& node_trace) mutable {
				if (ignore) {
					return false;
				}

				auto& [node, index] = node_trace;

				Status status;
				if (auto* composite = dynamic_cast<composite::Composite*>(node.get())) {
					status = composite->tick(tick_count, shared_from_this(), index);
				}
				else {
					status = node->tick(tick_count, shared_from_this());
				}
				
				if (status == Status::Success || status == Status::Failure) {
					return true;
				}
				else {
					ignore = true;
					return false;
				}
				};

			auto new_node_trace_list_end = std::remove_if(this->node_trace_list.begin(), this->node_trace_list.end(), tick_node_trace);
			this->node_trace_list.erase(
				new_node_trace_list_end,
				this->node_trace_list.end()
			);
		}
	}

	Status Context::UseRoot(const int& tick_count, const std::string& id) {
		return this->behaviour_tree->UseRoot(tick_count, shared_from_this(), id);
	}

	void Context::pushNodeTrace(std::pair<std::shared_ptr<node::Node>, int> node_trace)
	{
		this->node_trace_list.push_back(node_trace);
	}

	void Context::popNode()
	{
		this->node_trace_list.pop_back();
	}
}