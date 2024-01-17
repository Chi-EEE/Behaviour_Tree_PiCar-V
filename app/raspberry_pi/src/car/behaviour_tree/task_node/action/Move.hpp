#ifndef BEHAVIOUR_TREE_MOVE_HPP
#define BEHAVIOUR_TREE_MOVE_HPP

#pragma once

#include "behaviour_tree/task_node/Action.hpp"

namespace car::behaviour_tree::task_node::action
{
	class Move : public Action
	{
	public:
		Move(const int& speed) : speed(speed)
		{
		}

		const Status run(Context& context) override {
			/*using namespace car::behaviour_tree;
			CarContext &car_context = static_cast<CarContext&>(context);*/
			return Status::Success;
		}

		const std::string toString() const override {
			return fmt::format(R"(<Action:Move ms="{}"/>)", this->getSpeed());
		}

		const int& getSpeed() const {
			return this->speed;
		}

	private:
		const int speed;
	};
}

#endif