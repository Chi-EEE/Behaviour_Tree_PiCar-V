#ifndef BEHAVIOUR_TREE_SETREARWHEELDIRECTION_HPP
#define BEHAVIOUR_TREE_SETREARWHEELDIRECTION_HPP

#pragma once

#include "behaviour_tree/node/task/Action.hpp"

#include "../../../CarContext.cxx"

#include "DirectionType.hpp"
#include "WheelType.hpp"

namespace behaviour_tree::node::task::action
{
	class SetRearWheelDirection : public Action
	{
	public:
		SetRearWheelDirection(const std::string& name, const WheelType& wheel_type, const DirectionType& direction_type) : Action(name), wheel_type(wheel_type), direction_type(direction_type)
		{
		}

		const Status run(Context& context) override
		{
			CarContext& car_context = static_cast<CarContext&>(context);
			auto& car_system = car_context.getCarSystem();
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
				return fmt::format(R"(<Action:SetRearWheelDirection name="{}" direction="{}" wheel="{}"/>)", name, direction_type, wheel_type);
			else
				return fmt::format(R"(<Action:SetRearWheelDirection direction="{}" wheel="{}"/>)", direction_type, wheel_type);
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