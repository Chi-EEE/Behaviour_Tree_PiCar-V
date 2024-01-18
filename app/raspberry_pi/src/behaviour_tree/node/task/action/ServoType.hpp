#ifndef BEHAVIOUR_TREE_SERVOTYPE_HPP
#define BEHAVIOUR_TREE_SERVOTYPE_HPP

#pragma once

namespace behaviour_tree::node::task::action
{
    enum class ServoType
    {
        FrontWheels,
        CameraServo1,
        CameraServo2,
    };
}

#endif