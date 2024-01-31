#ifndef BEHAVIOUR_TREE_FAIL_HPP
#define BEHAVIOUR_TREE_FAIL_HPP

#pragma once

#include "Leaf.hpp"

namespace behaviour_tree::node::leaf
{
	class Fail final : public Leaf
	{
	public:
		Fail(const std::string& name) : Leaf(name) {}

		const LeafType type() const final override { return LeafType::Fail; }

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override
		{
			return Status::Failure;
		}

		const std::string toString() const final override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Fail name="{}"/>)", name);
			else
				return fmt::format(R"(<Fail/>)");
		}
	};
}

#endif