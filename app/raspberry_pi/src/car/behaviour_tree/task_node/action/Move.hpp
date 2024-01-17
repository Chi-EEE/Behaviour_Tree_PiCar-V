#ifndef BEHAVIOUR_TREE_MOVE_HPP
#define BEHAVIOUR_TREE_MOVE_HPP

#pragma once

#include "behaviour_tree/node/task/Action.hpp"

namespace car::behaviour_tree::task_node::action
{
	class Move : public Action
	{
	public:
		Move(const std::string& name, const int& speed) : Action(name), speed(speed)
		{
		}

		const Status run(Context& context) override {
			/*using namespace car::behaviour_tree;
			CarContext &car_context = static_cast<CarContext&>(context);*/
			return Status::Success;
		}

		const std::string toString() const override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:Move name="{}" speed="{}"/>)", name, this->getSpeed());
			else
				return fmt::format(R"(<Action:Move speed="{}"/>)", this->getSpeed());
		}

		const int& getSpeed() const {
			return this->speed;
		}

	private:
		const int speed;
	};
}

#endif