#ifndef BEHAVIOUR_TREE_TASK_HPP
#define BEHAVIOUR_TREE_TASK_HPP

#pragma once

#include <vector>

#include "Leaf.hpp"

#include "../../action/Action.hpp"

using namespace behaviour_tree::action;

namespace behaviour_tree::node::leaf
{
    class Task : public Leaf
    {
    public:
        Task(const std::string& name, std::vector<std::unique_ptr<Action>> actions) : Leaf(name), actions(std::move(actions))
        {
        }

        const LeafType type() const override { return LeafType::Task; }

        const Status run() override
        {
            for (auto &action : this->actions)
            {
                // TODO
                /*if (action->run() == Status::FAILURE) {
                    return Status::FAILURE;
                }*/
            }

            return Status::Success;
        }

        const std::string toString() const override {
            std::string out;
            for (auto& action : this->actions)
            {
                out += action->toString();
            }
            const std::string& name = this->getName();
            if (name != "")
                return fmt::format(R"(<Task name="{}">{}</Task>)", name, out);
            else
                return fmt::format(R"(<Task>{}</Task>)", out);
        }

        std::vector<std::unique_ptr<Action>> actions;
    };
}

#endif