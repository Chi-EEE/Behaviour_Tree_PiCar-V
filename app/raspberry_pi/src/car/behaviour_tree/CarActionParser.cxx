#ifndef BEHAVIOUR_TREE_CAR_ACTION_PARSER_HPP
#define BEHAVIOUR_TREE_CAR_ACTION_PARSER_HPP

#pragma once

#include <string>
#include <memory>

#include <tl/expected.hpp>
#include <pugixml.hpp>
#include <fmt/format.h>

#include "behaviour_tree/BehaviourTree.hpp"
#include "behaviour_tree/action/Action.hpp"
#include "behaviour_tree/action/ActionParser.hpp"

#include "action/Direction.hpp"
#include "action/Turn.hpp"
#include "action/Move.hpp"

namespace car::behaviour_tree
{
	// https://stackoverflow.com/a/46711735
	static constexpr uint32_t hash(const std::string_view s) noexcept
	{
		uint32_t hash = 5381;

		for (const char* c = s.data(); c < s.data() + s.size(); ++c)
			hash = ((hash << 5) + hash) + (unsigned char)*c;

		return hash;
	}

	class CarActionParser : public ActionParser
	{
	public:
		CarActionParser() {}

		tl::expected<std::unique_ptr<Action>, std::string> parseAction(pugi::xml_node& node) override
		{
			const std::string name = node.attribute("name").as_string();
			const std::string type = node.attribute("type").as_string();
			switch (hash(type))
			{
			case hash("Move"):
			{
				const int ms = node.attribute("ms").as_int();
				if (ms < 0)
					return tl::unexpected(fmt::format("Invalid ms: {} | Move", std::to_string(ms), node.name()));
				return std::make_unique<Move>(Move(ms));
			}
			case hash("Turn"):
			{
				const int angle = node.attribute("angle").as_int();
				if (angle < 0 || angle > 180)
					return tl::unexpected(fmt::format("Invalid angle: {} | Turn", std::to_string(angle), node.name()));
				return std::make_unique<Turn>(Turn(angle));
			}
			case hash("Direction"):
			{
				const std::string direction_type_attribute = node.attribute("direction_type").as_string();
				if (direction_type_attribute == "Forward")
					return std::make_unique<Direction>(Direction(DirectionType::Forward));
				else if (direction_type_attribute == "Backward")
					return std::make_unique<Direction>(Direction(DirectionType::Backward));
				else
					return tl::unexpected(fmt::format("Invalid direction_type: {} | Direction", direction_type_attribute, node.name()));
			}
			default:
			{
				return tl::unexpected(fmt::format("Invalid action type: {} | {}", type, node.name()));
			}
			}
		}
	};
}

#endif