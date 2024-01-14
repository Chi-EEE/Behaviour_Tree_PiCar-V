#ifndef BEHAVIOUR_TREE_TURN_HPP
#define BEHAVIOUR_TREE_TURN_HPP

#pragma once

#include "Action.hpp"

namespace behaviour_tree::action
{
    class Turn : public Action
    {
    public:
        Turn(const int &angle) : angle(angle)
        {
        }

        ActionType type() const override
        {
            return ActionType::Turn;
        }

    private:
        const int angle;
    };
}

#endif