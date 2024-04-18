#ifndef BEHAVIOUR_TREE_BLACKBOARD_TYPE_HPP
#define BEHAVIOUR_TREE_BLACKBOARD_TYPE_HPP

#pragma once

namespace behaviour_tree::node::blackboard
{
    enum class BlackboardType
    {
        ChangeInteger,
        IntegerCondition,
    };
}

#endif