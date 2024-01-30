#ifndef PLUGIN__CXX
#define PLUGIN__CXX

#pragma once

#include <memory>

namespace car::system {
	class CarSystem;
}

namespace car::plugin {
	class Plugin {
    public:
        virtual void initialize(std::shared_ptr<car::system::CarSystem> car_system) = 0;
        virtual void update() = 0;
        virtual void stop() = 0;
    };
}

#endif