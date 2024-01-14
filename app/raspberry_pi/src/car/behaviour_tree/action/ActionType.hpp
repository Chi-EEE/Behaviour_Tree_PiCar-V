#ifndef BEHAVIOUR_TREE_ACTIONTYPE_HPP
#define BEHAVIOUR_TREE_ACTIONTYPE_HPP

#pragma once

namespace behaviour_tree::action
{
    enum class ActionType
    {
        Move,
        Turn,
        Direction,
    };
}

#endif