#ifndef BEHAVIOUR_TREE_COMPOSITE_HPP
#define BEHAVIOUR_TREE_COMPOSITE_HPP

#pragma once

#include <vector>

#include "../Node.hpp"
#include "CompositeType.hpp"

namespace behaviour_tree::node::composite
{
	class Composite : public Node
	{
	public:
		Composite(const std::string& name, std::vector<std::shared_ptr<Node>> children) : Node(name), children(std::move(children))
		{
		}

		Status tick(const int& tick_count, std::shared_ptr<Context> context, const int& start_index)
		{
			assert(context != nullptr);

			if (!this->started) {
				this->start(context);
				this->started = true;
			}
			Status status = this->run(tick_count, context, start_index);
			if (status == Status::Success || status == Status::Failure) {
				this->finish(context);
				this->started = false;
			}
			return status;
		}

		virtual const Status run(const int& tick_count, std::shared_ptr<Context> context, const int& start_index) = 0;
		virtual const CompositeType type() const = 0;

	protected:
		std::vector<std::shared_ptr<Node>> children;
	};
}

#endif