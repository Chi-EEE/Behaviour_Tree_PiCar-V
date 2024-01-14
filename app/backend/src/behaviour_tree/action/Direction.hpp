#ifndef BEHAVIOUR_TREE_DIRECTION_HPP
#define BEHAVIOUR_TREE_DIRECTION_HPP

#pragma once

#include "Action.hpp"

#include "DirectionType.hpp"

namespace behaviour_tree::action
{
	class Direction : public Action
	{
	public:
		Direction(const DirectionType& direction_type) : direction_type(direction_type)
		{
		}

		ActionType type() const override
		{
			return ActionType::Direction;
		}

		const DirectionType& getDirectionType() const {
			return this->direction_type;
		}

		const std::string toString() const override {
			std::string direction_type;
			switch (this->getDirectionType()) {
			case DirectionType::Forward:
				direction_type = "Forward";
			case DirectionType::Backward:
				direction_type = "Backward";
			}
			return fmt::format(R"(<Direction direction="{}"/>)", direction_type);
		}

	private:
		const DirectionType direction_type;
	};
}

#endif