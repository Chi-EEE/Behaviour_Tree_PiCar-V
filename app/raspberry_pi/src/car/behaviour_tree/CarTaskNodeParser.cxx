#ifndef BEHAVIOUR_TREE_CAR_TASK_NODE_PARSER_HPP
#define BEHAVIOUR_TREE_CAR_TASK_NODE_PARSER_HPP

#pragma once

#include <string>
#include <memory>

#include <tl/expected.hpp>
#include <pugixml.hpp>
#include <fmt/format.h>

#include "behaviour_tree/BehaviourTree.hpp"
#include "behaviour_tree/task_node/TaskNode.hpp"
#include "behaviour_tree/task_node/Action.hpp"
#include "behaviour_tree/task_node/Condition.hpp"

#include "behaviour_tree/task_node/TaskNodeParser.hpp"

#include "task_node/action/Direction.hpp"
#include "task_node/action/Turn.hpp"
#include "task_node/action/Move.hpp"

#include "task_node/condition/NearbyPoints.hpp"

using namespace car::behaviour_tree::task_node;

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

	class CarTaskNodeParser : public TaskNodeParser
	{
	public:
		CarTaskNodeParser() {}

		tl::expected<std::unique_ptr<TaskNode>, std::string> parseTaskNode(pugi::xml_node& node) override
		{
			const std::string name = node.name();
			switch (hash(name))
			{
			case hash("Action:Move"):
			{
				const int speed = node.attribute("speed").as_int();
				if (speed < 0)
					return tl::unexpected(fmt::format("Invalid speed: {} | Action:Move", std::to_string(speed)));
				return std::make_unique<action::Move>(action::Move(speed));
			}
			case hash("Action:Turn"):
			{
				const int angle = node.attribute("angle").as_int();
				if (angle < 0 || angle > 180)
					return tl::unexpected(fmt::format("Invalid angle: {} | Action:Turn", std::to_string(angle)));
				return std::make_unique<action::Turn>(action::Turn(angle));
			}
			case hash("Action:Direction"):
			{
				const std::string direction_type_attribute = node.attribute("direction_type").as_string();
				if (direction_type_attribute == "Forward")
					return std::make_unique<action::Direction>(action::Direction(action::DirectionType::Forward));
				else if (direction_type_attribute == "Backward")
					return std::make_unique<action::Direction>(action::Direction(action::DirectionType::Backward));
				else
					return tl::unexpected(fmt::format("Invalid direction_type: {} | Action:Direction", direction_type_attribute));
			}
			case hash("Condition:NearbyPoints"):
			{
				const int min_angle = node.attribute("angle").as_int();
				if (min_angle < 0 || min_angle > 180)
					return tl::unexpected(fmt::format("Invalid min_angle: {} | Condition:NearbyPoints", std::to_string(min_angle)));
				const int max_angle = node.attribute("angle").as_int();
				if (max_angle < 0 || max_angle > 180)
					return tl::unexpected(fmt::format("Invalid max_angle: {} | Condition:NearbyPoints", std::to_string(max_angle)));
				const int avg_distance = node.attribute("angle").as_int();
				if (avg_distance < 0)
					return tl::unexpected(fmt::format("Invalid avg_distance: {} | Condition:NearbyPoints", std::to_string(avg_distance)));
				return std::make_unique<condition::NearbyPoints>(
					condition::NearbyPoints(
						min_angle,
						max_angle,
						avg_distance
					)
				);
			}
			default:
			{
				return tl::unexpected(fmt::format("Invalid task node type: {}", node.name()));
			}
			}
		}
	};
}

#endif