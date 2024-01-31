#ifndef BEHAVIOUR_TREE_UseRoot_HPP
#define BEHAVIOUR_TREE_UseRoot_HPP

#pragma once

#include "Leaf.hpp"

namespace behaviour_tree::node::leaf
{
	class UseRoot final : public Leaf
	{
	public:
		UseRoot(const std::string& name, const std::string& id) : Leaf(name), id(id) {}

		const LeafType type() const final override { return LeafType::UseRoot; }

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override
		{
			return context->UseRoot(tick_count, this->id);
		}

		const std::string& getId() const { return this->id; }

		const std::string toString() const final override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<UseRoot name="{}" id="{}"/>)", name, this->getId());
			else
				return fmt::format(R"(<UseRoot id="{}"/>)", this->getId());
		}

	private:
		const std::string id;
	};
}

#endif