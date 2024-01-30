#ifndef BEHAVIOUR_TREE_CARCONTEXT_HPP
#define BEHAVIOUR_TREE_CARCONTEXT_HPP

#pragma once

#include "../../src/car/system/CarSystem.h" // Might separate this file and others to move into the include folder.
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

        void log(std::string message)
        {
            this->messages.push_back(message);
        }

        void error(std::string message)
        {
            this->messages.push_back(message);
        }

        void _() override{};

    private:
        std::shared_ptr<car::system::CarSystem> car_system;
        std::vector<std::string> messages;
    };
}

#endif