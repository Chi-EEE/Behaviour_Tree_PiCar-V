#ifndef BEHAVIOUR_TREE_SUCCEED_ON_AVERAGE_ANY_SCAN_HPP
#define BEHAVIOUR_TREE_SUCCEED_ON_AVERAGE_ANY_SCAN_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#include <spdlog/spdlog.h>
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

#include "behaviour_tree/global/distance.h"

namespace behaviour_tree::node::custom::condition
{
    class SucceedOnAnyNearbyScan final : public CustomNode
    {
    public:
        SucceedOnAnyNearbyScan(const std::string &name, const int min_angle, const int max_angle, const double cm) : CustomNode(name),
                                                                                                                     min_angle(min_angle),
                                                                                                                     max_angle(max_angle),
                                                                                                                     cm(cm),
                                                                                                                     minimum_distance_unit(cm * global::CM_TO_DISTANCE)
        {
        }

        const static tl::expected<std::shared_ptr<SucceedOnAnyNearbyScan>, std::string> parse(const pugi::xml_node &node, const int index, const std::string &name_attribute)
        {
            const int min_angle = node.attribute("min_angle").as_int();
            if (min_angle < 0 || min_angle > 360)
            {
                return tl::unexpected(fmt::format(R"(Invalid min_angle: '{}' | Condition:SucceedOnAnyNearbyScan:['{}',{}])", min_angle, name_attribute, index));
            }
            const int max_angle = node.attribute("max_angle").as_int();
            if (max_angle < 0 || max_angle > 360)
            {
                return tl::unexpected(fmt::format(R"(Invalid max_angle: '{}' | Condition:SucceedOnAnyNearbyScan:['{}',{}])", max_angle, name_attribute, index));
            }
            const double cm = node.attribute("cm").as_double();
            if (cm < 0)
            {
                return tl::unexpected(fmt::format(R"(Invalid cm: '{}' | Condition:SucceedOnAnyNearbyScan:['{}',{}])", cm, name_attribute, index));
            }
            return std::make_shared<SucceedOnAnyNearbyScan>(
                SucceedOnAnyNearbyScan(
                    name_attribute,
                    min_angle,
                    max_angle,
                    cm));
        }

        const Status run(const int tick_count, std::shared_ptr<Context> context) final override
        {
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
            std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
            auto car_system = car_context->getCarSystem();
            std::vector<Measure> scan_data = car_system->getDeviceManager()->getLidarDevice()->getScanData();
            for (auto &measure : scan_data)
            {
                if (measure.angle > this->getMinAngle() && measure.angle < this->getMaxAngle())
                {
                    if (measure.distance < this->getMinimumDistanceUnit())
                    {
                        return Status::Success;
                    }
                }
            }
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
            return Status::Failure;
        }

        const int getMinAngle() const
        {
            return this->min_angle;
        }

        const int getMaxAngle() const
        {
            return this->max_angle;
        }

        const double getMinimumDistanceUnit() const
        {
            return this->minimum_distance_unit;
        }

        const double getCentimeters() const
        {
            return this->cm;
        }

        const std::string toString() const final override
        {
            const std::string &name = this->getName();
            if (name != "")
                return fmt::format(R"(<Condition:SucceedOnAnyNearbyScan name='{}' min_angle='{}' max_angle='{}' cm='{}'/>)", name, this->getMinAngle(), this->getMaxAngle(), this->getCentimeters());
            else
                return fmt::format(R"(<Condition:SucceedOnAnyNearbyScan min_angle='{}' max_angle='{}' cm='{}'/>)", this->getMinAngle(), this->getMaxAngle(), this->getCentimeters());
        }

    private:
        const int min_angle;
        const int max_angle;

        const double cm;

        const double minimum_distance_unit;
    };
}

#endif