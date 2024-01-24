#ifndef BEHAVIOUR_TREE_CAR_TASK_NODE_PARSER_HPP
#define BEHAVIOUR_TREE_CAR_TASK_NODE_PARSER_HPP

#pragma once

#include <string>
#include <memory>

#include <tl/expected.hpp>
#include <pugixml.hpp>
#include <fmt/format.h>

#include "behaviour_tree/BehaviourTree.hpp"
#include "behaviour_tree/node/custom/CustomNode.hpp"
#include "behaviour_tree/node/custom/Action.hpp"
#include "behaviour_tree/node/custom/Condition.hpp"

#include "behaviour_tree/node/custom/CustomNodeParser.hpp"

#include "action/Wait.hpp"

#include "action/Log.hpp"
#include "action/Error.hpp"

#include "action/SetSpeed.hpp"
#include "action/SetWheelDirection.hpp"
#include "action/SetAngle.hpp"

#include "condition/NearbyPoints.hpp"

#include "utils/Utility.hpp"

namespace behaviour_tree::node::custom
{
	class CarCustomNodeParser : public custom::CustomNodeParser
	{
	public:
		CarCustomNodeParser() {}

		tl::expected<std::unique_ptr<custom::CustomNode>, std::string> parseCustomNode(const pugi::xml_node& node, const int& index) override
		{
			const std::string name_attribute = node.attribute("node_name").as_string();
			const std::string node_name = node.name();
			switch (utils::Utility::hash(node_name))
			{
			case utils::Utility::hash("Action:Wait"):
			{
				return std::make_unique<custom::action::Wait>(
					custom::action::Wait(
						name_attribute,
						node.attribute("ms").as_int()));
			}
			case utils::Utility::hash("Action:Log"):
			{
				return std::make_unique<custom::action::Log>(
					custom::action::Log(
						name_attribute,
						node.attribute("text").as_string()));
			}
			case utils::Utility::hash("Action:Error"):
			{
				return std::make_unique<custom::action::Error>(
					custom::action::Error(
						name_attribute,
						node.attribute("text").as_string()));
			}
			case utils::Utility::hash("Action:SetSpeed"):
			{
				const std::string wheel_type_attribute = node.attribute("wheel_type").as_string();
				custom::action::WheelType wheel_type;
				switch (utils::Utility::hash(wheel_type_attribute))
				{
				case utils::Utility::hash("Left"):
				{
					wheel_type = custom::action::WheelType::Left;
					break;
				}
				case utils::Utility::hash("Right"):
				{
					wheel_type = custom::action::WheelType::Right;
					break;
				}
				case utils::Utility::hash("Both"):
				{
					wheel_type = custom::action::WheelType::Both;
					break;
				}
				default:
				{
					return tl::unexpected(fmt::format(R"(Invalid wheel_type: '{}' | Action:SetSpeed:["{}",{}])", wheel_type_attribute, name_attribute, index));
				}
				}
				const int speed = node.attribute("speed").as_int();
				if (speed < 0 || speed > 100)
					return tl::unexpected(fmt::format(R"(Invalid speed: '{}' | Action:SetSpeed:["{}",{}])", speed, name_attribute, index));
				return std::make_unique<custom::action::SetSpeed>(custom::action::SetSpeed(name_attribute, wheel_type, speed));
			}
			case utils::Utility::hash("Action:SetAngle"):
			{
				const std::string servo_type_attribute = node.attribute("servo_type").as_string();
				custom::action::ServoType servo_type;
				switch (utils::Utility::hash(servo_type_attribute))
				{
				case utils::Utility::hash("FrontWheels"):
				{
					servo_type = custom::action::ServoType::FrontWheels;
					break;
				}
				case utils::Utility::hash("CameraServo1"):
				{
					servo_type = custom::action::ServoType::CameraServo1;
					break;
				}
				case utils::Utility::hash("CameraServo2"):
				{
					servo_type = custom::action::ServoType::CameraServo2;
					break;
				}
				default:
				{
					return tl::unexpected(fmt::format(R"(Invalid servo_type: '{}' | Action:SetAngle:["{}",{}])", servo_type_attribute, name_attribute, index));
				}
				}
				const int angle = node.attribute("angle").as_int();
				if (angle < 0 || angle > 180)
					return tl::unexpected(fmt::format(R"(Invalid angle: '{}' | Action:SetAngle:["{}",{}])", angle, name_attribute, index));
				return std::make_unique<custom::action::SetAngle>(custom::action::SetAngle(name_attribute, servo_type, angle));
			}
			case utils::Utility::hash("Action:SetWheelDirection"):
			{
				const std::string wheel_type_attribute = node.attribute("wheel_type").as_string();
				custom::action::WheelType wheel_type;
				switch (utils::Utility::hash(wheel_type_attribute))
				{
				case utils::Utility::hash("Left"):
				{
					wheel_type = custom::action::WheelType::Left;
					break;
				}
				case utils::Utility::hash("Right"):
				{
					wheel_type = custom::action::WheelType::Right;
					break;
				}
				case utils::Utility::hash("Both"):
				{
					wheel_type = custom::action::WheelType::Both;
					break;
				}
				default:
				{
					return tl::unexpected(fmt::format(R"(Invalid wheel_type: '{}' | Action:SetWheelDirection:["{}",{}])", wheel_type_attribute, name_attribute, index));
				}
				}
				const std::string direction_type_attribute = node.attribute("direction_type").as_string();
				switch (utils::Utility::hash(direction_type_attribute))
				{
				case utils::Utility::hash("Forward"):
				{
					return std::make_unique<custom::action::SetWheelDirection>(custom::action::SetWheelDirection(name_attribute, wheel_type, custom::action::DirectionType::Forward));
				}
				case utils::Utility::hash("Backward"):
				{
					return std::make_unique<custom::action::SetWheelDirection>(custom::action::SetWheelDirection(name_attribute, wheel_type, custom::action::DirectionType::Backward));
				}
				default:
				{
					return tl::unexpected(fmt::format(R"(Invalid direction_type: '{}' | Action:SetWheelDirection:["{}",{}])", direction_type_attribute, name_attribute, index));
				}
				}
			}
			case utils::Utility::hash("Condition:NearbyPoints"):
			{
				const int min_angle = node.attribute("angle").as_int();
				if (min_angle < 0 || min_angle > 180)
					return tl::unexpected(fmt::format(R"(Invalid min_angle: '{}' | Condition:NearbyPoints:["{}",{}])", min_angle, name_attribute, index));
				const int max_angle = node.attribute("angle").as_int();
				if (max_angle < 0 || max_angle > 180)
					return tl::unexpected(fmt::format(R"(Invalid max_angle: '{}' | Condition:NearbyPoints:["{}",{}])", max_angle, name_attribute, index));
				const int avg_distance = node.attribute("angle").as_int();
				if (avg_distance < 0)
					return tl::unexpected(fmt::format(R"(Invalid avg_distance: '{}' | Condition:NearbyPoints:["{}",{}])", avg_distance, name_attribute, index));
				return std::make_unique<custom::condition::NearbyPoints>(
					custom::condition::NearbyPoints(
						name_attribute,
						min_angle,
						max_angle,
						avg_distance));
			}
			default:
			{
				return tl::unexpected(fmt::format(R"(Invalid custom node type: '{}' | {}:["{}",{}])", node_name, node_name, name_attribute, index));
			}
			}
		}
	};
}

#endif