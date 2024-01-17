#ifndef BEHAVIOUR_TREE_TURN_HPP
#define BEHAVIOUR_TREE_TURN_HPP

#pragma once

#include <string>

#include <fmt/format.h>

#include "behaviour_tree/Context.h"
#include "behaviour_tree/task_node/Action.hpp"

using namespace behaviour_tree;
using namespace behaviour_tree::task_node;

namespace car::behaviour_tree::task_node::action
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
            return fmt::format(R"(<Action:Turn angle="{}"/>)", this->getAngle());
        }

        const int& getAngle() const {
            return this->angle;
        }

    private:
        const int angle;
    };
}

#endif