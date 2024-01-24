#include "Context.h"
#include "BehaviourTree.hpp"

namespace behaviour_tree {
	Context::Context(std::shared_ptr<BehaviourTree> behaviour_tree) : behaviour_tree(behaviour_tree)
	{
	}

	Status Context::toRoot(const std::string& id) {
		return this->behaviour_tree->toRoot(*this, id);
	}
}