#ifndef BEHAVIOUR_TREE_INTEGER_CHANGE_TYPE_HPP
#define BEHAVIOUR_TREE_INTEGER_CHANGE_TYPE_HPP

#pragma once

namespace behaviour_tree::node::blackboard
{
    enum class IntegerChangeType
    {
        Set,
        Add,
        Subtract,
        Multiply,
        Divide,
    };
}

#endif