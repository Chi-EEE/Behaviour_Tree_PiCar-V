#ifndef BEHAVIOUR_TREE_ROOT_HPP
#define BEHAVIOUR_TREE_ROOT_HPP

#pragma once

#include <string>

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

		const Status run()
		{
			return this->child->run();
		}

		const std::string &getId() const
		{
			return this->id;
		}

	private:
		std::string id;
		std::shared_ptr<Node> child;
	};
}

#endif