#ifndef BEHAVIOUR_TREE_LEAF_HPP
#define BEHAVIOUR_TREE_LEAF_HPP

#pragma once

#include "../Node.hpp"

#include "LeafType.hpp"

namespace behaviour_tree::node::leaf
{
    class Leaf : public Node
    {
    public:
        Leaf(std::string name) : Node(name)
        {
        }

        virtual const LeafType type() const = 0;

    };
}

#endif