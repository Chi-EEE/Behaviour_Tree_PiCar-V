#ifndef BEHAVIOUR_TREE_SETWHEELDIRECTION_HPP
#define BEHAVIOUR_TREE_SETWHEELDIRECTION_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

#include "DirectionType.hpp"
#include "WheelType.hpp"

namespace behaviour_tree::node::custom::action
{
	class SetWheelDirection final : public CustomNode
	{
	public:
		SetWheelDirection(const std::string& name, const WheelType& wheel_type, const DirectionType& direction_type) : CustomNode(name), wheel_type(wheel_type), direction_type(direction_type)
		{
		}

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
			auto car_system = car_context->getCarSystem();
			const MovementSystem* movement_system = car_system->getMovementSystem();
			switch (this->getDirectionType()) {
			case DirectionType::Forward:
			{
				switch (this->getWheelType()) {
				case WheelType::Left:
				{
					movement_system->setRearLeftWheelDirectionToForward();
					break;
				}
				case WheelType::Right:
				{
					movement_system->setRearRightWheelDirectionToForward();
					break;
				}
				case WheelType::Both:
				{
					movement_system->setRearWheelsDirectionToForward();
					break;
				}
				}
				break;
			}
			case DirectionType::Backward:
			{
				switch (this->getWheelType()) {
				case WheelType::Left:
				{
					movement_system->setRearLeftWheelDirectionToBackward();
					break;
				}
				case WheelType::Right:
				{
					movement_system->setRearRightWheelDirectionToBackward();
					break;
				}
				case WheelType::Both:
				{
					movement_system->setRearWheelsDirectionToBackward();
					break;
				}
				}
				break;
			}
			}
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
		}

		const std::string toString() const final override {
			std::string direction_type;
			switch (this->getDirectionType()) {
			case DirectionType::Forward:
				direction_type = "Forward";
				break;
			case DirectionType::Backward:
				direction_type = "Backward";
				break;
			}
			std::string wheel_type;
			switch (this->getWheelType())
			{
			case WheelType::Left:
				wheel_type = "Forward";
				break;
			case WheelType::Right:
				wheel_type = "Backward";
				break;
			case WheelType::Both:
				wheel_type = "Both";
				break;
			}
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:SetWheelDirection name='{}' direction_type='{}' wheel_type='{}'/>)", name, direction_type, wheel_type);
			else
				return fmt::format(R"(<Action:SetWheelDirection direction_type='{}' wheel_type='{}'/>)", direction_type, wheel_type);
		}

		const DirectionType& getDirectionType() const {
			return this->direction_type;
		}

		const WheelType& getWheelType() const
		{
			return this->wheel_type;
		}

	private:
		const DirectionType direction_type;
		const WheelType wheel_type;
	};
}

#endif