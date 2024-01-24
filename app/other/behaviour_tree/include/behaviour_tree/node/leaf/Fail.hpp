#ifndef BEHAVIOUR_TREE_FAIL_HPP
#define BEHAVIOUR_TREE_FAIL_HPP

#pragma once

#include "Leaf.hpp"

namespace behaviour_tree::node::leaf
{
	class Fail : public Leaf
	{
	public:
		Fail(const std::string& name) : Leaf(name) {}

		const LeafType type() const override { return LeafType::Fail; }

		const Status tick(const int& tick_count, std::shared_ptr<Context> context) override
		{
			return Status::Failure;
		}

		const std::string toString() const override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Fail name="{}"/>)", name);
			else
				return fmt::format(R"(<Fail/>)");
		}
	};
}

#endif