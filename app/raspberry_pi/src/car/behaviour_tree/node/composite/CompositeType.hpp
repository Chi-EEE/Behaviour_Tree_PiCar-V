#ifndef BEHAVIOUR_TREE_COMPOSITETYPE_HPP
#define BEHAVIOUR_TREE_COMPOSITETYPE_HPP

#pragma once

namespace behaviour_tree::node::composite
{
    enum class CompositeType
    {
        Sequence,
        Selector,
    };
}

#endif