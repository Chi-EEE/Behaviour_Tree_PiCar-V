#ifndef BEHAVIOUR_TREE_CONDITION_OPERATOR_TYPE_HPP
#define BEHAVIOUR_TREE_CONDITION_OPERATOR_TYPE_HPP

#pragma once

namespace behaviour_tree::node::blackboard
{
    enum class ConditionOperatorType
    {
        Equal,
        NotEqual,
        GreaterThan,
        GreaterThanOrEqual,
        LessThan,
        LessThanOrEqual,
    };
}

#endif