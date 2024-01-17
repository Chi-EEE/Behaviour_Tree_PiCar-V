#ifndef BEHAVIOUR_TREE_TASK_HPP
#define BEHAVIOUR_TREE_TASK_HPP

#pragma once

#include <vector>

#include "Leaf.hpp"

#include "../../task_node/TaskNode.hpp"

using namespace behaviour_tree::task_node;

namespace behaviour_tree::node::leaf
{
    class Task : public Leaf
    {
    public:
        Task(const std::string& name, std::vector<std::unique_ptr<TaskNode>> task_nodes) : Leaf(name), task_nodes(std::move(task_nodes))
        {
        }

        const LeafType type() const override { return LeafType::Task; }

        const Status run(Context& context) override
        {
            for (auto & task_node : this->task_nodes)
            {
                if (task_node->run(context) == Status::Failure) {
                    return Status::Failure;
                }
            }

            return Status::Success;
        }

        const std::string toString() const override {
            std::string out;
            for (auto& task_node : this->task_nodes)
            {
                out += task_node->toString();
            }
            const std::string& name = this->getName();
            if (name != "")
                return fmt::format(R"(<Task name="{}">{}</Task>)", name, out);
            else
                return fmt::format(R"(<Task>{}</Task>)", out);
        }

        std::vector<std::unique_ptr<TaskNode>> task_nodes;
    };
}

#endif