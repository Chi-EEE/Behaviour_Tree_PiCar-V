#ifndef BEHAVIOUR_TREE_TURNAROUND_HPP
#define BEHAVIOUR_TREE_TURNAROUND_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

#include "enum/ClockDirectionType.hpp"

namespace behaviour_tree::node::custom::action
{
	class TurnAround final : public CustomNode
	{
	public:
		TurnAround(const std::string& name, const ClockDirectionType& clock_direction_type) : CustomNode(name), clock_direction_type(clock_direction_type)
		{
		}

		const Status run(const int tick_count, std::shared_ptr<Context> context) final override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
			auto car_system = car_context->getCarSystem();
			const MovementSystem* movement_system = car_system->getMovementSystem();
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
		}

		const std::string toString() const final override
		{
			std::string clock_direction_type = [this]()
				{
					switch (this->getClockDirectionType())
					{
					case ClockDirectionType::Clockwise:
						return "Clockwise";
					case ClockDirectionType::AntiClockwise:
						return "AntiClockwise";
					default:
						return "Unknown";
					}
				}
			();

			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:TurnAround name='{}' clock_direction_type='{}'/>)", name, clock_direction_type);
			else
				return fmt::format(R"(<Action:TurnAround clock_direction_type='{}'/>)", clock_direction_type);
		}

		const ClockDirectionType& getClockDirectionType() const
		{
			return this->clock_direction_type;
		}

	private:
		const ClockDirectionType clock_direction_type;
	};
}

#endif