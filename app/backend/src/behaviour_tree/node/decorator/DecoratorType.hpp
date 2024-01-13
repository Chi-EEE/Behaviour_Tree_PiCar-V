#ifndef BEHAVIOUR_TREE_DECORATORTYPE_HPP
#define BEHAVIOUR_TREE_DECORATORTYPE_HPP

#pragma once

namespace behaviour_tree::node::decorator
{
    enum class DecoratorType
    {
        Invert,
        Repeat,
        Wait,
    };
}

#endif