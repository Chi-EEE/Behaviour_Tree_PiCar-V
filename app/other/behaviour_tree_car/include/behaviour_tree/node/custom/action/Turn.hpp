#ifndef BEHAVIOUR_TREE_TURN_HPP
#define BEHAVIOUR_TREE_TURN_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN


namespace behaviour_tree::node::custom::action
{
	class Turn final : public CustomNode
	{
	public:
		Turn(const std::string &name, const int angle) : CustomNode(name), angle(angle)
		{
		}

		const static tl::expected<std::shared_ptr<Turn>, std::string> parse(const pugi::xml_node &node, const int index, const std::string &name_attribute)
		{
			const int angle = node.attribute("angle").as_int();
			if (angle < 0 || angle > 180)
			{
				return tl::unexpected(fmt::format(R"(Invalid angle: '{}' | Action:SetAngle:['{}',{}])", angle, name_attribute, index));
			}
			return std::make_shared<Turn>(Turn(name_attribute, angle));
		}

		const Status run(const int tick_count, std::shared_ptr<Context> context) final override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
			auto car_system = car_context->getCarSystem();
			const MovementSystem *movement_system = car_system->getMovementSystem();
            movement_system->setFrontWheelsAngle(this->getAngle());
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
		}

		const std::string toString() const final override
		{
			const std::string &name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:Turn name='{}' angle='{}'/>)", name, this->getAngle());
			else
				return fmt::format(R"(<Action:Turn angle='{}'/>)", this->getAngle());
		}

		const int getAngle() const
		{
			return this->angle;
		}

	private:
		const int angle;
	};
}

#endif