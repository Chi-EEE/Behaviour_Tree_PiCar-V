#ifndef BEHAVIOUR_TREE_BLACKBOARD_LEAF_HPP
#define BEHAVIOUR_TREE_BLACKBOARD_LEAF_HPP

#pragma once

#include "../Node.hpp"

namespace behaviour_tree::node::blackboard
{
    class BlackboardLeaf : public Node
    {
    public:
        BlackboardLeaf(const std::string& name) : Node(name)
        {
        }
    };
}

#endif