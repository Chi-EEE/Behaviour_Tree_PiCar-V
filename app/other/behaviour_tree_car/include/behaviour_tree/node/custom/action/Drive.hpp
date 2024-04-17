#ifndef BEHAVIOUR_TREE_DRIVE_HPP
#define BEHAVIOUR_TREE_DRIVE_HPP

#pragma once

#include <fmt/format.h>

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

#include "enum/DirectionType.hpp"

namespace behaviour_tree::node::custom::action
{
    class Drive final : public CustomNode
    {
    public:
        Drive(const std::string &name, const int speed, const DirectionType direction_type) : CustomNode(name), speed(speed), direction_type(direction_type)
        {
        }

        const static tl::expected<std::shared_ptr<Drive>, std::string> parse(const pugi::xml_node &node, const int index, const std::string &name_attribute)
        {
            const int speed = node.attribute("speed").as_int();
            if (speed < 0 || speed > 100)
            {
                return tl::unexpected(fmt::format(R"(Invalid speed: '{}' | Action:Drive:['{}',{}])", speed, name_attribute, index));
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
                    result = tl::make_unexpected(fmt::format(R"(Invalid direction_type: '{}' | Action:Drive:['{}',{}])", direction_type_attribute, name_attribute, index));
                    break;
                }
                };
                return result;
            }();
            if (!maybe_direction_type.has_value())
            {
                return tl::make_unexpected(maybe_direction_type.error());
            }
            return std::make_shared<Drive>(
                Drive(
                    name_attribute,
                    speed,
                    maybe_direction_type.value()));
        }

        const Status run(const int tick_count, std::shared_ptr<Context> context) final override
        {
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
            std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
            auto car_system = car_context->getCarSystem();
            const MovementSystem *movement_system = car_system->getMovementSystem();
            switch (this->getDirectionType())
            {
            case DirectionType::Backward:
            {
                movement_system->setRearWheelsDirectionToBackward();
                break;
            }
            case DirectionType::Forward:
            {
                movement_system->setRearWheelsDirectionToForward();
                break;
            }
            }
            movement_system->setRearWheelsSpeed(this->getSpeed());
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
            return Status::Success;
        }

        const std::string toString() const final override
        {
            const std::string &name = this->getName();
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
            if (name != "")
                return fmt::format(R"(<Action:Drive name='{}' speed='{}' direction_type='{}'/>)", name, this->getSpeed());
            else
                return fmt::format(R"(<Action:Drive speed='{}' direction_type='{}'/>)", this->getSpeed());
        }

        const int getSpeed() const
        {
            return this->speed;
        }

        const DirectionType getDirectionType() const
        {
            return this->direction_type;
        }

    private:
        const int speed;
        const DirectionType direction_type;
    };
}

#endif