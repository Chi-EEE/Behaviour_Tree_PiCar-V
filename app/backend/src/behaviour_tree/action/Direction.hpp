#ifndef BEHAVIOUR_TREE_DIRECTION_HPP
#define BEHAVIOUR_TREE_DIRECTION_HPP

#pragma once

#include "Action.hpp"

#include "DirectionType.hpp"

namespace behaviour_tree::action
{
    class Direction : public Action
    {
    public:
        Direction(DirectionType direction_type) : direction_type(direction_type)
        {
        }

        ActionType type() const override
        {
            return ActionType::Direction;
        }

    private:
        DirectionType direction_type;
    };
}

#endif