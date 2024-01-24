#ifndef BEHAVIOUR_TREE_SETDIRECTION_HPP
#define BEHAVIOUR_TREE_SETDIRECTION_HPP

#pragma once

#include "behaviour_tree/node/custom/Action.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

#include "DirectionType.hpp"
#include "WheelType.hpp"

namespace behaviour_tree::node::custom::action
{
	class SetDirection : public Action
	{
	public:
		SetDirection(const std::string& name, const WheelType& wheel_type, const DirectionType& direction_type) : Action(name), wheel_type(wheel_type), direction_type(direction_type)
		{
		}

		const Status run(Context& context) override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			CarContext& car_context = static_cast<CarContext&>(context);
			auto car_system = car_context.getCarSystem();
			switch (this->getDirectionType()) {
			case DirectionType::Forward:
			{
				switch (this->getWheelType()) {
				case WheelType::Left:
				{
					car_system->setRearLeftWheelDirectionToForward();
					break;
				}
				case WheelType::Right:
				{
					car_system->setRearRightWheelDirectionToForward();
					break;
				}
				case WheelType::Both:
				{
					car_system->setRearWheelsDirectionToForward();
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
					car_system->setRearLeftWheelDirectionToBackward();
					break;
				}
				case WheelType::Right:
				{
					car_system->setRearRightWheelDirectionToBackward();
					break;
				}
				case WheelType::Both:
				{
					car_system->setRearWheelsDirectionToBackward();
					break;
				}
				}
				break;
			}
			}
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
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
				return fmt::format(R"(<Action:SetDirection name="{}" direction_type="{}" wheel_type="{}"/>)", name, direction_type, wheel_type);
			else
				return fmt::format(R"(<Action:SetDirection direction_type="{}" wheel_type="{}"/>)", direction_type, wheel_type);
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