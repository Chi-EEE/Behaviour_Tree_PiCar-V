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

#include "behaviour_tree/node/custom/CustomNodeParser.hpp"

#include "action/PauseExecution.hpp"

#include "action/Print.hpp"

#include "action/Drive.hpp"
#include "action/SetSpeed.hpp"
#include "action/SetWheelDirection.hpp"
#include "action/SetAngle.hpp"
#include "action/TurnAround.hpp"

#include "condition/SucceedOnAverageNearbyScan.hpp"
#include "condition/SucceedOnAnyNearbyScan.hpp"
#include "condition/SucceedOnAverageColour.hpp"

#include "utils/Utility.hpp"

namespace behaviour_tree::node::custom
{
    class CarCustomNodeParser : public custom::CustomNodeParser
    {
    public:
        CarCustomNodeParser() {}

        tl::expected<std::shared_ptr<custom::CustomNode>, std::string> parseCustomNode(const pugi::xml_node &node, const int index) override
        {
            const std::string name_attribute = node.attribute("node_name").as_string();
            const std::string node_name = node.name();
            switch (utils::hash(node_name))
            {
            case utils::hash("Action:PauseExecution"):
            {
                int ms = node.attribute("ms").as_int();
                if (ms < 0)
                {
                    return tl::unexpected(fmt::format(R"(Invalid ms: '{}' | Action:PauseExecution:['{}',{}])", ms, name_attribute, index));
                }
                return std::make_shared<custom::action::PauseExecution>(
                    custom::action::PauseExecution(
                        name_attribute,
                        ms));
            }
            case utils::hash("Action:Print"):
            {
                return std::make_shared<custom::action::Print>(
                    custom::action::Print(
                        name_attribute,
                        node.attribute("text").as_string()));
            }
            case utils::hash("Action:Drive"):
            {
                const int speed = node.attribute("speed").as_int();
                if (speed < 0 || speed > 100)
                {
                    return tl::unexpected(fmt::format(R"(Invalid speed: '{}' | Action:Drive:['{}',{}])", speed, name_attribute, index));
                }
                const tl::expected<custom::action::DirectionType, std::string> maybe_direction_type =
                    [&]()
                {
                    const std::string direction_type_attribute = node.attribute("direction_type").as_string();
                    tl::expected<custom::action::DirectionType, std::string> result;
                    switch (utils::hash(direction_type_attribute))
                    {
                    case utils::hash("Forward"):
                    {
                        result = custom::action::DirectionType::Forward;
                    }
                    case utils::hash("Backward"):
                    {
                        result = custom::action::DirectionType::Backward;
                    }
                    default:
                    {
                        result = tl::make_unexpected(fmt::format(R"(Invalid direction_type: '{}' | Action:Drive:['{}',{}])", direction_type_attribute, name_attribute, index));
                    }
                    };
                    return result;
                }();
                if (!maybe_direction_type.has_value())
                {
                    return tl::make_unexpected(maybe_direction_type.error());
                }
                return std::make_shared<custom::action::Drive>(
                    custom::action::Drive(
                        name_attribute,
                        speed,
                        maybe_direction_type.value()));
            }
            case utils::hash("Action:SetSpeed"):
            {
                const tl::expected<custom::action::WheelType, std::string> maybe_wheel_type = [&]()
                {
                    const std::string wheel_type_attribute = node.attribute("wheel_type").as_string();
                    tl::expected<custom::action::WheelType, std::string> result;
                    switch (utils::hash(wheel_type_attribute))
                    {
                    case utils::hash("Left"):
                    {
                        result = custom::action::WheelType::Left;
                        break;
                    }
                    case utils::hash("Right"):
                    {
                        result = custom::action::WheelType::Right;
                        break;
                    }
                    case utils::hash("Both"):
                    {
                        result = custom::action::WheelType::Both;
                        break;
                    }
                    default:
                    {
                        result = tl::unexpected(fmt::format(R"(Invalid wheel_type: '{}' | Action:SetSpeed:['{}',{}])", wheel_type_attribute, name_attribute, index));
                    }
                    };
                    return result;
                }();
                if (!maybe_wheel_type.has_value())
                {
                    return tl::make_unexpected(maybe_wheel_type.error());
                }
                const int speed = node.attribute("speed").as_int();
                if (speed < 0 || speed > 100)
                {
                    return tl::unexpected(fmt::format(R"(Invalid speed: '{}' | Action:SetSpeed:['{}',{}])", speed, name_attribute, index));
                }
                return std::make_shared<custom::action::SetSpeed>(custom::action::SetSpeed(name_attribute, maybe_wheel_type.value(), speed));
            }
            case utils::hash("Action:SetAngle"):
            {
                const tl::expected<custom::action::ServoType, std::string> maybe_servo_type = [&]()
                {
                    const std::string servo_type_attribute = node.attribute("servo_type").as_string();
                    tl::expected<custom::action::ServoType, std::string> result;
                    switch (utils::hash(servo_type_attribute))
                    {
                    case utils::hash("FrontWheels"):
                    {
                        result = custom::action::ServoType::FrontWheels;
                        break;
                    }
                    case utils::hash("CameraServo1"):
                    {
                        result = custom::action::ServoType::CameraServo1;
                        break;
                    }
                    case utils::hash("CameraServo2"):
                    {
                        result = custom::action::ServoType::CameraServo2;
                        break;
                    }
                    default:
                    {
                        result = tl::unexpected(fmt::format(R"(Invalid servo_type: '{}' | Action:SetAngle:['{}',{}])", servo_type_attribute, name_attribute, index));
                    }
                    };
                    return result;
                }();
                const int angle = node.attribute("angle").as_int();
                if (angle < 0 || angle > 180)
                    return tl::unexpected(fmt::format(R"(Invalid angle: '{}' | Action:SetAngle:['{}',{}])", angle, name_attribute, index));
                return std::make_shared<custom::action::SetAngle>(custom::action::SetAngle(name_attribute, maybe_servo_type.value(), angle));
            }
            case utils::hash("Action:SetWheelDirection"):
            {
                const tl::expected<custom::action::WheelType, std::string> maybe_wheel_type = [&]()
                {
                    const std::string wheel_type_attribute = node.attribute("wheel_type").as_string();
                    tl::expected<custom::action::WheelType, std::string> result;
                    switch (utils::hash(wheel_type_attribute))
                    {
                    case utils::hash("Left"):
                    {
                        result = custom::action::WheelType::Left;
                        break;
                    }
                    case utils::hash("Right"):
                    {
                        result = custom::action::WheelType::Right;
                        break;
                    }
                    case utils::hash("Both"):
                    {
                        result = custom::action::WheelType::Both;
                        break;
                    }
                    default:
                    {
                        result = tl::unexpected(fmt::format(R"(Invalid wheel_type: '{}' | Action:SetWheelDirection:['{}',{}])", wheel_type_attribute, name_attribute, index));
                    }
                    };
                    return result;
                }();
                if (!maybe_wheel_type.has_value())
                {
                    return tl::make_unexpected(maybe_wheel_type.error());
                }
                const tl::expected<custom::action::DirectionType, std::string> maybe_direction_type =
                    [&]()
                {
                    const std::string direction_type_attribute = node.attribute("direction_type").as_string();
                    tl::expected<custom::action::DirectionType, std::string> result;
                    switch (utils::hash(direction_type_attribute))
                    {
                    case utils::hash("Forward"):
                    {
                        result = custom::action::DirectionType::Forward;
                    }
                    case utils::hash("Backward"):
                    {
                        result = custom::action::DirectionType::Backward;
                    }
                    default:
                    {
                        result = tl::make_unexpected(fmt::format(R"(Invalid direction_type: '{}' | Action:Drive:['{}',{}])", direction_type_attribute, name_attribute, index));
                    }
                    };
                    return result;
                }();
                if (!maybe_direction_type.has_value())
                {
                    return tl::make_unexpected(maybe_direction_type.error());
                }
                return std::make_shared<custom::action::SetWheelDirection>(custom::action::SetWheelDirection(name_attribute, maybe_wheel_type.value(), maybe_direction_type.value()));
            }
            case utils::hash("Condition:SucceedOnAverageNearbyScan"):
            {
                const int min_angle = node.attribute("min_angle").as_int();
                if (min_angle < 0 || min_angle > 360)
                    return tl::unexpected(fmt::format(R"(Invalid min_angle: '{}' | Condition:SucceedOnAverageNearbyScan:['{}',{}])", min_angle, name_attribute, index));
                const int max_angle = node.attribute("max_angle").as_int();
                if (max_angle < 0 || max_angle > 360)
                    return tl::unexpected(fmt::format(R"(Invalid max_angle: '{}' | Condition:SucceedOnAverageNearbyScan:['{}',{}])", max_angle, name_attribute, index));
                const double cm = node.attribute("cm").as_double();
                if (cm < 0)
                    return tl::unexpected(fmt::format(R"(Invalid cm: '{}' | Condition:SucceedOnAverageNearbyScan:['{}',{}])", cm, name_attribute, index));
                const int minimum_measure_amount_used = node.attribute("minimum_measure_amount_used").as_int();
                if (minimum_measure_amount_used < 0)
                    return tl::unexpected(fmt::format(R"(Invalid minimum_measure_amount_used: '{}' | Condition:SucceedOnAverageNearbyScan:['{}',{}])", minimum_measure_amount_used, name_attribute, index));
                return std::make_shared<custom::condition::SucceedOnAverageNearbyScan>(
                    custom::condition::SucceedOnAverageNearbyScan(
                        name_attribute,
                        min_angle,
                        max_angle,
                        cm,
                        minimum_measure_amount_used));
            }
            case utils::hash("Condition:SucceedOnAnyNearbyScan"):
            {
                const int min_angle = node.attribute("min_angle").as_int();
                if (min_angle < 0 || min_angle > 360)
                    return tl::unexpected(fmt::format(R"(Invalid min_angle: '{}' | Condition:SucceedOnAnyNearbyScan:['{}',{}])", min_angle, name_attribute, index));
                const int max_angle = node.attribute("max_angle").as_int();
                if (max_angle < 0 || max_angle > 360)
                    return tl::unexpected(fmt::format(R"(Invalid max_angle: '{}' | Condition:SucceedOnAnyNearbyScan:['{}',{}])", max_angle, name_attribute, index));
                const double cm = node.attribute("cm").as_double();
                if (cm < 0)
                    return tl::unexpected(fmt::format(R"(Invalid cm: '{}' | Condition:SucceedOnAnyNearbyScan:['{}',{}])", cm, name_attribute, index));
                return std::make_shared<custom::condition::SucceedOnAnyNearbyScan>(
                    custom::condition::SucceedOnAnyNearbyScan(
                        name_attribute,
                        min_angle,
                        max_angle,
                        cm));
            }
            case utils::hash("Condition:SucceedOnAverageColour"):
            {
                const std::string hex = node.attribute("hex").as_string();
                const double percentage = node.attribute("percentage").as_double();
                if (percentage < 0 || percentage > 100)
                    return tl::unexpected(fmt::format(R"(Invalid percentage: '{}' | Condition:SucceedOnAverageColour:['{}',{}])", percentage, name_attribute, index));
                return std::make_shared<custom::condition::SucceedOnAverageColour>(
                    custom::condition::SucceedOnAverageColour(
                        name_attribute,
                        hex,
                        percentage));
            }
            // case utils::hash("Action:TurnAround"):
            // {
            // 	const std::string direction_type_attribute = node.attribute("direction_type").as_string();
            // 	switch (utils::hash(direction_type_attribute))
            // 	{
            // 	case utils::hash("Clockwise"):
            // 	{
            // 		return std::make_shared<custom::action::TurnAround>(custom::action::TurnAround(name_attribute, custom::action::ClockDirectionType::Clockwise));
            // 	}
            // 	case utils::hash("AntiClockwise"):
            // 	{
            // 		return std::make_shared<custom::action::TurnAround>(custom::action::TurnAround(name_attribute, custom::action::ClockDirectionType::AntiClockwise));
            // 	}
            // 	default:
            // 	{
            // 		return tl::unexpected(fmt::format(R"(Invalid direction_type: '{}' | Action:TurnAround:['{}',{}])", direction_type_attribute, name_attribute, index));
            // 	}
            // 	}
            // }
            default:
            {
                return tl::unexpected(fmt::format(R"(Invalid custom node type: '{}' | {}:['{}',{}])", node_name, node_name, name_attribute, index));
            }
            }
        }
    };
}

#endif