#ifndef BEHAVIOUR_TREE_SUCCEED_HPP
#define BEHAVIOUR_TREE_SUCCEED_HPP

#pragma once

#include "Leaf.hpp"

namespace behaviour_tree::node::leaf
{
	class Succeed : public Leaf
	{
	public:
		Succeed(const std::string& name) : Leaf(name) {}

		const LeafType type() const final override { return LeafType::Succeed; }

		const Status tick(const int& tick_count, std::shared_ptr<Context> context) final override
		{
			return Status::Success;
		}

		const std::string toString() const final override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Succeed name="{}"/>)", name);
			else
				return fmt::format(R"(<Succeed/>)");
		}
	};
}

#endif