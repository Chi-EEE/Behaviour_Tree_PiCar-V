#ifndef BEHAVIOUR_TREE_DIRECTION_HPP
#define BEHAVIOUR_TREE_DIRECTION_HPP

#pragma once

#include "behaviour_tree/task_node/Action.hpp"

#include "behaviour_tree/Context.h"

#include "DirectionType.hpp"

using namespace behaviour_tree;
using namespace behaviour_tree::task_node;

namespace car::behaviour_tree::task_node::action
{
	class Direction : public Action
	{
	public:
		Direction(const DirectionType& direction_type) : direction_type(direction_type)
		{
		}

		const Status run(Context& context) override
		{
			/*using namespace car::behaviour_tree;
			CarContext &car_context = static_cast<CarContext&>(context);
			context.setDirection(this->getDirectionType());*/
			return Status::Success;
		}

		const DirectionType& getDirectionType() const {
			return this->direction_type;
		}

		const std::string toString() const override {
			std::string direction_type;
			switch (this->getDirectionType()) {
			case DirectionType::Forward:
				direction_type = "Forward";
				break;
			case DirectionType::Backward:
				direction_type = "Backward";
				break;
			}
			return fmt::format(R"(<Action:Direction direction="{}"/>)", direction_type);
		}

	private:
		const DirectionType direction_type;
	};
}

#endif