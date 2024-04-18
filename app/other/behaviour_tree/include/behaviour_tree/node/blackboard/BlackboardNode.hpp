#ifndef BEHAVIOUR_TREE_BLACKBOARD_NODE_HPP
#define BEHAVIOUR_TREE_BLACKBOARD_NODE_HPP

#pragma once

#include "../Node.hpp"

#include "BlackboardType.hpp"

namespace behaviour_tree::node::blackboard
{
    class BlackboardNode : public Node
    {
    public:
        BlackboardNode(const std::string& name) : Node(name)
        {
        }

        virtual const BlackboardType type() const = 0;
    };
}

#endif