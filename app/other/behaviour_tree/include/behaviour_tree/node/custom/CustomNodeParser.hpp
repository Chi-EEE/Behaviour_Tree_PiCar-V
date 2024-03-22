#ifndef BEHAVIOUR_TREE_CUSTOM_NODE_PARSER_HPP
#define BEHAVIOUR_TREE_CUSTOM_NODE_PARSER_HPP

#pragma once

#include <string>
#include <memory>

#include <pugixml.hpp>
#include <tl/expected.hpp>
#include <fmt/format.h>

#include "CustomNode.hpp"

namespace behaviour_tree::node::custom
{
	class CustomNodeParser
	{
	public:
		virtual tl::expected<std::shared_ptr<CustomNode>, std::string> parseCustomNode(const pugi::xml_node& node, const int index) = 0;
	};
}

#endif