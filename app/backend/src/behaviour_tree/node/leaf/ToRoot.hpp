#ifndef BEHAVIOUR_TREE_TOROOT_HPP
#define BEHAVIOUR_TREE_TOROOT_HPP

#pragma once

#include "Leaf.hpp"

namespace behaviour_tree::node::leaf
{
	class ToRoot : public Leaf
	{
	public:
		ToRoot(const std::string& name, const std::string &id) : Leaf(name), id(id) {}

		const LeafType type() const override { return LeafType::ToRoot; }

		const Status run() override
		{
			// TODO
			return Status::Success;
		}

		const std::string &getId() const { return this->id; }

		const std::string toString() const override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<ToRoot name="{}" id="{}"/>)", name, this->getId());
			else
				return fmt::format(R"(<ToRoot id="{}"/>)", this->getId());
		}

	private:
		const std::string id;
	};
}

#endif