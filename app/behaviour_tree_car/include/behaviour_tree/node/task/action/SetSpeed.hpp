#ifndef BEHAVIOUR_TREE_SETSPEED_HPP
#define BEHAVIOUR_TREE_SETSPEED_HPP

#pragma once

#include "behaviour_tree/node/task/Action.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "../../../CarContext.cxx"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

#include "WheelType.hpp"

namespace behaviour_tree::node::task::action
{
	class SetSpeed : public Action
	{
	public:
		SetSpeed(const std::string& name, const WheelType& wheel_type, const int& speed) : Action(name), wheel_type(wheel_type), speed(speed)
		{
		}

		const Status run(Context& context) override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			CarContext& car_context = static_cast<CarContext&>(context);
			auto& car_system = car_context.getCarSystem();
			switch (this->getWheelType())
			{
			case WheelType::Left:
				car_system->setRearLeftWheelSpeed(this->getSpeed());
				break;
			case WheelType::Right:
				car_system->setRearRightWheelSpeed(this->getSpeed());
				break;
			case WheelType::Both:
				car_system->setRearWheelsSpeed(this->getSpeed());
				break;
			}
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
		}

		const std::string toString() const override
		{
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
				return fmt::format(R"(<Action:SetSpeed name="{}" wheel_type="{}" speed="{}"/>)", name, wheel_type, this->getSpeed());
			else
				return fmt::format(R"(<Action:SetSpeed wheel_type="{}" speed="{}"/>)", wheel_type, this->getSpeed());
		}

		const WheelType& getWheelType() const
		{
			return this->wheel_type;
		}

		const int& getSpeed() const
		{
			return this->speed;
		}

	private:
		const int speed;
		const WheelType wheel_type;
	};
}

#endif