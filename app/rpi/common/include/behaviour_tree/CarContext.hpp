#ifndef BEHAVIOUR_TREE_CARCONTEXT_HPP
#define BEHAVIOUR_TREE_CARCONTEXT_HPP

#pragma once

#include "car/system/CarSystem.h"
#include "behaviour_tree/Context.h"

namespace behaviour_tree
{
    class CarContext : public Context
    {
    public:
        CarContext(std::shared_ptr<BehaviourTree> behaviour_tree, std::shared_ptr<car::system::CarSystem> car_system) : Context(std::move(behaviour_tree)), car_system(std::move(car_system))
        {
        }

        std::shared_ptr<car::system::CarSystem> getCarSystem() const
        {
            return this->car_system;
        }

        void _() override{};

    private:
        std::shared_ptr<car::system::CarSystem> car_system;
    };
}

#endif