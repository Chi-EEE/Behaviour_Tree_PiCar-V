#ifndef BEHAVIOUR_TREE_MOVE_HPP
#define BEHAVIOUR_TREE_MOVE_HPP

#pragma once

#include "Action.hpp"

namespace behaviour_tree::action
{
    class Move : public Action
    {
    public:
        Move(const int &ms) : ms(ms)
        {
        }

        ActionType type() const override
        {
            return ActionType::Move;
        }

    private:
        const int ms;
    };
}

#endif