#ifndef BEHAVIOUR_TREE_CARCONTEXT_CXX
#define BEHAVIOUR_TREE_CARCONTEXT_CXX

#pragma once

#include "../system/CarSystem.h"
#include "behaviour_tree/Context.hpp"

using namespace car::system;
using namespace behaviour_tree;

namespace car::behaviour_tree
{
    class CarContext
    {
    public:
        CarContext(std::shared_ptr<CarSystem> car_system) : car_system(std::move(car_system))
        {
        }

    private:
        std::shared_ptr<CarSystem> car_system;
    };
}

#endif