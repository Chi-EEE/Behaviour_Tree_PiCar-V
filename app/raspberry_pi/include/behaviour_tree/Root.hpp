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
		Root(std::string id, std::shared_ptr<Node> child) : id(id), child(std::move(child))
		{
		}

		const Status run(Context& context)
		{
			return this->child->run(context);
		}

		const std::string toString() const {
			const std::string& id = this->getId();
			if (id != "")
				return fmt::format(R"(<Root id="{}">{}</Root>)", id, this->child->toString());
			else
				return fmt::format(R"(<Root>{}</Root>)", this->child->toString());
		}

		const std::string& getId() const
		{
			return this->id;
		}

	private:
		std::string id;
		std::shared_ptr<Node> child;
	};
}

#endif