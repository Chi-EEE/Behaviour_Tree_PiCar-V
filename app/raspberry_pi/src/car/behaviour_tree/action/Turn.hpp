#ifndef BEHAVIOUR_TREE_TURN_HPP
#define BEHAVIOUR_TREE_TURN_HPP

#pragma once

#include <string>

#include <fmt/format.h>

#include "behaviour_tree/Context.h"
#include "behaviour_tree/action/Action.hpp"

using namespace behaviour_tree;
using namespace behaviour_tree::action;

namespace behaviour_tree::action
{
    class Turn : public Action
    {
    public:
        Turn(const int &angle) : angle(angle)
        {
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