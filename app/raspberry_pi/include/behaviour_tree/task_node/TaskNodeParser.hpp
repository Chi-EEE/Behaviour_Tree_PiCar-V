#ifndef BEHAVIOUR_TREE_TASK_NODE_PARSER_HPP
#define BEHAVIOUR_TREE_TASK_NODE_PARSER_HPP

#pragma once

#include <string>
#include <memory>

#include <pugixml.hpp>
#include <tl/expected.hpp>
#include <fmt/format.h>

#include "TaskNode.hpp"

namespace behaviour_tree::task_node
{
	// https://stackoverflow.com/a/46711735
	static constexpr uint32_t hash(const std::string_view s) noexcept
	{
		uint32_t hash = 5381;

		for (const char* c = s.data(); c < s.data() + s.size(); ++c)
			hash = ((hash << 5) + hash) + (unsigned char)*c;

		return hash;
	}

	class TaskNodeParser
	{
	public:
		virtual tl::expected<std::unique_ptr<TaskNode>, std::string> parseTaskNode(pugi::xml_node& node) = 0;
	};
}

#endif