#ifndef BEHAVIOUR_TREE_ROOT_HPP
#define BEHAVIOUR_TREE_ROOT_HPP

#pragma once

#include <string>
#include <fmt/format.h>

#include "node/Node.hpp"

using namespace behaviour_tree::node;

namespace behaviour_tree
{
	class Root
	{
	public:
		Root(std::string id, std::shared_ptr<node::Node> child) : id(id), child(std::move(child))
		{
		}

		const Status tick(Context& context)
		{
			if (this->child == nullptr) return Status::Success;
			return this->child->tick(context);
		}

		const std::string toString() const {
			const std::string& id = this->getId();
			std::string child_string = this->child != nullptr ? this->child->toString() : "";
			if (id != "")
				return fmt::format(R"(<Root id="{}">{}</Root>)", id, child_string);
			else
				return fmt::format(R"(<Root>{}</Root>)", child_string);
		}

		const std::string& getId() const
		{
			return this->id;
		}

	private:
		std::string id;
		std::shared_ptr<node::Node> child;
	};
}

#endif