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
                return custom::action::PauseExecution::parse(node, index, name_attribute);
            case utils::hash("Action:Print"):
                return custom::action::Print::parse(node, index, name_attribute);
            case utils::hash("Action:Drive"):
                return custom::action::Drive::parse(node, index, name_attribute);
            case utils::hash("Action:SetSpeed"):
                return custom::action::SetSpeed::parse(node, index, name_attribute);
            case utils::hash("Action:SetAngle"):
                return custom::action::SetAngle::parse(node, index, name_attribute);
            case utils::hash("Action:SetWheelDirection"):
                return custom::action::SetWheelDirection::parse(node, index, name_attribute);
            case utils::hash("Condition:SucceedOnAverageNearbyScan"):
                return custom::condition::SucceedOnAverageNearbyScan::parse(node, index, name_attribute);
            case utils::hash("Condition:SucceedOnAnyNearbyScan"):
                return custom::condition::SucceedOnAnyNearbyScan::parse(node, index, name_attribute);
            case utils::hash("Condition:SucceedOnAverageColour"):
                return custom::condition::SucceedOnAverageColour::parse(node, index, name_attribute);
            default:
            {
                return tl::unexpected(fmt::format(R"(Invalid custom node type: '{}' | {}:['{}',{}])", node_name, node_name, name_attribute, index));
            }
            }
        }
    };
}

#endif