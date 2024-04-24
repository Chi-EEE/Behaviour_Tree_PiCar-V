#ifndef BEHAVIOUR_TREE_SETWHEELDIRECTION_HPP
#define BEHAVIOUR_TREE_SETWHEELDIRECTION_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

#include "enum/DirectionType.hpp"
#include "enum/WheelType.hpp"

namespace behaviour_tree::node::custom::action
{
	class SetWheelDirection final : public CustomNode
	{
	public:
		SetWheelDirection(const std::string &name, const WheelType wheel_type, const DirectionType direction_type) : CustomNode(name), wheel_type(wheel_type), direction_type(direction_type)
		{
		}

		const static tl::expected<std::shared_ptr<SetWheelDirection>, std::string> parse(const pugi::xml_node &node, const int index, const std::string &name_attribute)
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
					result = tl::unexpected(fmt::format(R"(Invalid wheel_type: '{}' | Action:SetWheelDirection:['{}',{}])", wheel_type_attribute, name_attribute, index));
                    break;
                }
				};
				return result;
			}();
			if (!maybe_wheel_type.has_value())
			{
				return tl::make_unexpected(maybe_wheel_type.error());
			}
			const tl::expected<DirectionType, std::string> maybe_direction_type =
				[&]()
			{
				const std::string direction_type_attribute = node.attribute("direction_type").as_string();
				tl::expected<DirectionType, std::string> result;
				switch (utils::hash(direction_type_attribute))
				{
				case utils::hash("Forward"):
				{
					result = DirectionType::Forward;
                    break;
				}
				case utils::hash("Backward"):
				{
					result = DirectionType::Backward;
                    break;
				}
				default:
				{
					result = tl::make_unexpected(fmt::format(R"(Invalid direction_type: '{}' | Action:SetWheelDirection:['{}',{}])", direction_type_attribute, name_attribute, index));
                    break;
                }
				};
				return result;
			}();
			if (!maybe_direction_type.has_value())
			{
				return tl::make_unexpected(maybe_direction_type.error());
			}
			return std::make_shared<SetWheelDirection>(SetWheelDirection(name_attribute, maybe_wheel_type.value(), maybe_direction_type.value()));
		}

		const Status run(const int tick_count, std::shared_ptr<Context> context) final override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
			auto car_system = car_context->getCarSystem();
			const MovementSystem *movement_system = car_system->getMovementSystem();
			switch (this->getDirectionType())
			{
			case DirectionType::Forward:
			{
				switch (this->getWheelType())
				{
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
				switch (this->getWheelType())
				{
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

		const std::string toString() const final override
		{
			std::string direction_type;
			switch (this->getDirectionType())
			{
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
			const std::string &name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:SetWheelDirection name='{}' direction_type='{}' wheel_type='{}'/>)", name, direction_type, wheel_type);
			else
				return fmt::format(R"(<Action:SetWheelDirection direction_type='{}' wheel_type='{}'/>)", direction_type, wheel_type);
		}

		const DirectionType getDirectionType() const
		{
			return this->direction_type;
		}

		const WheelType getWheelType() const
		{
			return this->wheel_type;
		}

	private:
		const DirectionType direction_type;
		const WheelType wheel_type;
	};
}

#endif