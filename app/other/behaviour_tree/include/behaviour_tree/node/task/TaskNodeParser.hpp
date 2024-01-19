#ifndef BEHAVIOUR_TREE_TASK_NODE_PARSER_HPP
#define BEHAVIOUR_TREE_TASK_NODE_PARSER_HPP

#pragma once

#include <string>
#include <memory>

#include <pugixml.hpp>
#include <tl/expected.hpp>
#include <fmt/format.h>

#include "TaskNode.hpp"

namespace behaviour_tree::node::task
{
	class TaskNodeParser
	{
	public:
		virtual tl::expected<std::unique_ptr<TaskNode>, std::string> parseTaskNode(const pugi::xml_node& node) = 0;
	};
}

#endif