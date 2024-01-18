#ifndef BEHAVIOUR_TREE_STATUS_HPP
#define BEHAVIOUR_TREE_STATUS_HPP

#pragma once

namespace behaviour_tree
{
    enum class Status
    {
        Success,
        Failure,
        Running
    };
}

#endif