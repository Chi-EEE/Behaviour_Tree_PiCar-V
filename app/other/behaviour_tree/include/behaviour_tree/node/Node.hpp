#ifndef BEHAVIOUR_TREE_NODE_HPP
#define BEHAVIOUR_TREE_NODE_HPP

#pragma once

#include <string>
#include <memory>

#include <fmt/format.h>

#include "../Status.hpp"
#include "../Context.h"

namespace behaviour_tree::node
{
	class Node
	{
	public:
		Node(const std::string& name) : name(name)
		{
		}

        virtual void start(std::shared_ptr<Context> context) {}
		virtual const Status run(const int& tick_count, std::shared_ptr<Context> context) = 0;
        virtual void finish(std::shared_ptr<Context> context) {}

        Status tick(const int& tick_count, std::shared_ptr<Context> context)
        {
            if (!this->started) {
                this->start(context);
                this->started = true;
            }
            Status status = this->tick(tick_count, context);
            if (status == Status::Success || status == Status::Failure) {
                this->finish(context);
                this->started = false;
            }
            return status;
        }

		virtual const std::string toString() const = 0;

		const std::string& getName() const
		{
			return this->name;
		}

	protected:
		const std::string name;

    private:
        bool started = false;
	};
}

#endif