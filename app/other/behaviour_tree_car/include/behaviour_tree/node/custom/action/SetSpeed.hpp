#ifndef BEHAVIOUR_TREE_SETSPEED_HPP
#define BEHAVIOUR_TREE_SETSPEED_HPP

#pragma once

#include <fmt/format.h>

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

#include "enum/WheelType.hpp"

namespace behaviour_tree::node::custom::action
{
	class SetSpeed final : public CustomNode
	{
	public:
		SetSpeed(const std::string &name, const WheelType wheel_type, const int speed) : CustomNode(name), wheel_type(wheel_type), speed(speed)
		{
		}

		const static tl::expected<std::shared_ptr<SetSpeed>, std::string> parse(const pugi::xml_node &node, const int index, const std::string &name_attribute)
		{
			const tl::expected<WheelType, std::string> maybe_wheel_type = [&]()
			{
				const std::string wheel_type_attribute = node.attribute("wheel_type").as_string();
				tl::expected<WheelType, std::string> result;
				switch (utils::hash(wheel_type_attribute))
				{
				case utils::hash("Left"):
				{
					result = WheelType::Left;
					break;
				}
				case utils::hash("Right"):
				{
					result = WheelType::Right;
					break;
				}
				case utils::hash("Both"):
				{
					result = WheelType::Both;
					break;
				}
				default:
				{
					result = tl::unexpected(fmt::format(R"(Invalid wheel_type: '{}' | Action:SetSpeed:['{}',{}])", wheel_type_attribute, name_attribute, index));
                    break;
                }
				};
				return result;
			}();
			if (!maybe_wheel_type.has_value())
			{
				return tl::make_unexpected(maybe_wheel_type.error());
			}
			const int speed = node.attribute("speed").as_int();
			if (speed < 0 || speed > 100)
			{
				return tl::unexpected(fmt::format(R"(Invalid speed: '{}' | Action:SetSpeed:['{}',{}])", speed, name_attribute, index));
			}
			return std::make_shared<SetSpeed>(SetSpeed(name_attribute, maybe_wheel_type.value(), speed));
		}

		const Status run(const int tick_count, std::shared_ptr<Context> context) final override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
			auto car_system = car_context->getCarSystem();
			const MovementSystem *movement_system = car_system->getMovementSystem();
			switch (this->getWheelType())
			{
			case WheelType::Left:
				movement_system->setRearLeftWheelSpeed(this->getSpeed());
				break;
			case WheelType::Right:
				movement_system->setRearRightWheelSpeed(this->getSpeed());
				break;
			case WheelType::Both:
				movement_system->setRearWheelsSpeed(this->getSpeed());
				break;
			}
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
		}

		const std::string toString() const final override
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
			const std::string &name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:SetSpeed name='{}' wheel_type='{}' speed='{}'/>)", name, wheel_type, this->getSpeed());
			else
				return fmt::format(R"(<Action:SetSpeed wheel_type='{}' speed='{}'/>)", wheel_type, this->getSpeed());
		}

		const WheelType getWheelType() const
		{
			return this->wheel_type;
		}

		const int getSpeed() const
		{
			return this->speed;
		}

	private:
		const int speed;
		const WheelType wheel_type;
	};
}

#endif