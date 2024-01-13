#ifndef BEHAVIOUR_TREE_STATUS_HPP
#define BEHAVIOUR_TREE_STATUS_HPP

#pragma once

namespace behaviour_tree::node
{
    enum class Status
    {
        SUCCESS,
        FAILURE,
        RUNNING
    };
}

#endif