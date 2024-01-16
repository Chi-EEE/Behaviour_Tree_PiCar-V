#ifndef BEHAVIOUR_TREE_CARCONTEXT_CXX
#define BEHAVIOUR_TREE_CARCONTEXT_CXX

#pragma once

#include "../system/CarSystem.h"
#include "behaviour_tree/Context.h"

using namespace car::system;
using namespace behaviour_tree;

namespace car::behaviour_tree
{
    class CarContext : public Context
    {
    public:
        CarContext(std::shared_ptr<BehaviourTree> behaviour_tree, std::shared_ptr<CarSystem> car_system) : Context(std::move(behaviour_tree)), car_system(std::move(car_system))
        {
        }

    private:
        std::shared_ptr<CarSystem> car_system;
    };
}

#endif