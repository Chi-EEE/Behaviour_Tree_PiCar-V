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

        const ActionType type() const override
        {
            return ActionType::Turn;
        }

        const Status run(Context& context) override {
            /*using namespace car::behaviour_tree;
            CarContext &car_context = static_cast<CarContext&>(context);*/
            return Status::Success;
        }

        const std::string toString() const override {
            return fmt::format(R"(<Action type="Turn" angle="{}"/>)", this->getAngle());
        }

        const int& getAngle() const {
            return this->angle;
        }

    private:
        const int angle;
    };
}

#endif