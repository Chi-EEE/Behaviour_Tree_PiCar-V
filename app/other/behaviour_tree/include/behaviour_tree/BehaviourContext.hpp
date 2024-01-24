#ifndef BEHAVIOUR_CONTEXT_HPP
#define BEHAVIOUR_CONTEXT_HPP

#pragma once

#include "BehaviourTree.hpp"
#include "Context.h"

namespace behaviour_tree
{
    struct BehaviourContext
    {
        std::shared_ptr<BehaviourTree> behaviour_tree;
        Context context;
    };
}

#endif