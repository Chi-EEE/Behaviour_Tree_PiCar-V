#ifndef BEHAVIOUR_TREE_CHANGE_INTEGER_HPP
#define BEHAVIOUR_TREE_CHANGE_INTEGER_HPP

#pragma once

#include "BlackboardLeaf.hpp"

#include "enum/ConditionOperatorType.hpp"

namespace behaviour_tree::node::blackboard
{
    class IntegerCondition final : public BlackboardLeaf
    {
    public:
        IntegerCondition(const std::string &name) : BlackboardLeaf(name) {}

        const Status run(const int tick_count, std::shared_ptr<Context> context) final override
        {
        }

        const std::string toString() const final override
        {
            const std::string &name = this->getName();
            const std::string condition_operator = [&]() {
                switch (this->condition_operator)
                {
                case ConditionOperatorType::Equal:
                    return "=";
                case ConditionOperatorType::GreaterThan:
                    return ">";
                case ConditionOperatorType::GreaterThanOrEqual:
                    return ">=";
                case ConditionOperatorType::LessThan:
                    return "<";
                case ConditionOperatorType::LessThanOrEqual:
                    return "<=";
                case ConditionOperatorType::NotEqual:
                    return "!=";
                default:
                    return "Invalid";
                }
                }
            ();
            if (name != "")
                return fmt::format(R"(<Blackboard:IntegerCondition name='{}' variable_name='{}' condition_operator='{}' value='{}'/>)", name, this->variable_name, condition_operator, this->value);
            else
                return fmt::format(R"(<Blackboard:IntegerCondition variable_name='{}' condition_operator='{}' value='{}'/>)", this->variable_name, condition_operator, this->value);
        }

    private:
        const std::string variable_name;
        const ConditionOperatorType condition_operator;
        const int value;
    };
}

#endif