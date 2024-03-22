#include "Context.h"
#include "BehaviourTree.hpp"
#include "node/Node.hpp"
#include "node/composite/Composite.hpp"

namespace behaviour_tree {
	Context::Context(std::shared_ptr<BehaviourTree> behaviour_tree) : behaviour_tree(behaviour_tree)
	{
		this->behaviour_tree->start();
	}

	void Context::update(const int tick_count)
	{
		if (this->node_trace_list.size() == 0) {
			this->behaviour_tree->tick(tick_count, shared_from_this());
		}
		else {
			std::vector<std::pair<std::shared_ptr<node::Node>, int>> node_trace_list_copy = this->node_trace_list;
			std::vector<std::pair<std::shared_ptr<node::Node>, int>>::iterator i = node_trace_list_copy.begin();
			Status status = Status::Running;
			while (i != node_trace_list_copy.end()) {
				std::shared_ptr<node::Node> node = i->first;
				int index = i->second;

				if (std::shared_ptr<composite::Composite> composite = std::dynamic_pointer_cast<composite::Composite>(node)) {
					status = composite->tick(tick_count, shared_from_this(), index);
				}
				else {
					status = node->tick(tick_count, shared_from_this());
				}

				if (status == Status::Running) {
					break;
				}
				++i;
			}
			if (status != Status::Running) {
				this->behaviour_tree->incrementCycle();
			}
		}
	}

	Status Context::UseRoot(const int tick_count, const std::string& id) {
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

	bool Context::canRun() const {
		return this->behaviour_tree->canRun();
	}
}