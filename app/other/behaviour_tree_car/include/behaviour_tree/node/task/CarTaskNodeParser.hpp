#ifndef BEHAVIOUR_TREE_CAR_TASK_NODE_PARSER_HPP
#define BEHAVIOUR_TREE_CAR_TASK_NODE_PARSER_HPP

#pragma once

#include <string>
#include <memory>

#include <tl/expected.hpp>
#include <pugixml.hpp>
#include <fmt/format.h>

#include "behaviour_tree/BehaviourTree.hpp"
#include "behaviour_tree/node/task/TaskNode.hpp"
#include "behaviour_tree/node/task/Action.hpp"
#include "behaviour_tree/node/task/Condition.hpp"

#include "behaviour_tree/node/task/TaskNodeParser.hpp"

#include "action/Wait.hpp"

#include "action/SetSpeed.hpp"
#include "action/SetDirection.hpp"
#include "action/SetAngle.hpp"

#include "condition/NearbyPoints.hpp"

#include "utils/Utility.hpp"

namespace behaviour_tree::node::task
{
	class CarTaskNodeParser : public task::TaskNodeParser
	{
	public:
		CarTaskNodeParser() {}

		tl::expected<std::unique_ptr<task::TaskNode>, std::string> parseTaskNode(const pugi::xml_node& node) override
		{
			const std::string name_attribute = node.attribute("name").as_string();
			const std::string name = node.name();
			switch (utils::Utility::hash(name))
			{
			case utils::Utility::hash("Action:Wait"):
			{
				return std::make_unique<task::action::Wait>(
					task::action::Wait(
						name,
						node.attribute("ms").as_int()));
			}
			case utils::Utility::hash("Action:SetSpeed"):
			{
				const std::string wheel_type_attribute = node.attribute("wheel_type").as_string();
				task::action::WheelType wheel_type;
				switch (utils::Utility::hash(wheel_type_attribute))
				{
				case utils::Utility::hash("Left"):
				{
					wheel_type = task::action::WheelType::Left;
					break;
				}
				case utils::Utility::hash("Right"):
				{
					wheel_type = task::action::WheelType::Right;
					break;
				}
				case utils::Utility::hash("Both"):
				{
					wheel_type = task::action::WheelType::Both;
					break;
				}
				default:
				{
					return tl::unexpected(fmt::format("Invalid wheel_type: '{}' | Action:SetSpeed", wheel_type_attribute));
				}
				}
				const int speed = node.attribute("speed").as_int();
				if (speed < 0 || speed > 100)
					return tl::unexpected(fmt::format("Invalid speed: '{}' | Action:SetSpeed", std::to_string(speed)));
				return std::make_unique<task::action::SetSpeed>(task::action::SetSpeed(name_attribute, wheel_type, speed));
			}
			case utils::Utility::hash("Action:SetAngle"):
			{
				const std::string servo_type_attribute = node.attribute("servo_type").as_string();
				task::action::ServoType servo_type;
				switch (utils::Utility::hash(servo_type_attribute))
				{
				case utils::Utility::hash("FrontWheels"):
				{
					servo_type = task::action::ServoType::FrontWheels;
					break;
				}
				case utils::Utility::hash("CameraServo1"):
				{
					servo_type = task::action::ServoType::CameraServo1;
					break;
				}
				case utils::Utility::hash("CameraServo2"):
				{
					servo_type = task::action::ServoType::CameraServo2;
					break;
				}
				default:
				{
					return tl::unexpected(fmt::format("Invalid servo_type: '{}' | Action:SetAngle", servo_type_attribute));
				}
				}
				const int angle = node.attribute("angle").as_int();
				if (angle < 0 || angle > 180)
					return tl::unexpected(fmt::format("Invalid angle: '{}' | Action:SetAngle", std::to_string(angle)));
				return std::make_unique<task::action::SetAngle>(task::action::SetAngle(name_attribute, servo_type, angle));
			}
			case utils::Utility::hash("Action:SetDirection"):
			{
				const std::string wheel_type_attribute = node.attribute("wheel_type").as_string();
				task::action::WheelType wheel_type;
				switch (utils::Utility::hash(wheel_type_attribute))
				{
				case utils::Utility::hash("Left"):
				{
					wheel_type = task::action::WheelType::Left;
					break;
				}
				case utils::Utility::hash("Right"):
				{
					wheel_type = task::action::WheelType::Right;
					break;
				}
				case utils::Utility::hash("Both"):
				{
					wheel_type = task::action::WheelType::Both;
					break;
				}
				default:
				{
					return tl::unexpected(fmt::format("Invalid wheel_type: '{}' | Action:SetDirection", wheel_type_attribute));
				}
				}
				const std::string direction_type_attribute = node.attribute("direction_type").as_string();
				switch (utils::Utility::hash(direction_type_attribute))
				{
				case utils::Utility::hash("Forward"):
				{
					return std::make_unique<task::action::SetDirection>(task::action::SetDirection(name_attribute, wheel_type, task::action::DirectionType::Forward));
				}
				case utils::Utility::hash("Backward"):
				{
					return std::make_unique<task::action::SetDirection>(task::action::SetDirection(name_attribute, wheel_type, task::action::DirectionType::Backward));
				}
				default:
				{
					return tl::unexpected(fmt::format("Invalid direction_type: '{}' | Action:SetDirection", direction_type_attribute));
				}
				}
			}
			case utils::Utility::hash("Condition:NearbyPoints"):
			{
				const int min_angle = node.attribute("angle").as_int();
				if (min_angle < 0 || min_angle > 180)
					return tl::unexpected(fmt::format("Invalid min_angle: '{}' | Condition:NearbyPoints", std::to_string(min_angle)));
				const int max_angle = node.attribute("angle").as_int();
				if (max_angle < 0 || max_angle > 180)
					return tl::unexpected(fmt::format("Invalid max_angle: '{}' | Condition:NearbyPoints", std::to_string(max_angle)));
				const int avg_distance = node.attribute("angle").as_int();
				if (avg_distance < 0)
					return tl::unexpected(fmt::format("Invalid avg_distance: '{}' | Condition:NearbyPoints", std::to_string(avg_distance)));
				return std::make_unique<task::condition::NearbyPoints>(
					task::condition::NearbyPoints(
						name_attribute,
						min_angle,
						max_angle,
						avg_distance));
			}
			default:
			{
				return tl::unexpected(fmt::format("Invalid task node type: '{}'", node.name()));
			}
			}
		}
	};
}

#endif