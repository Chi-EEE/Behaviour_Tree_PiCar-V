#ifndef BEHAVIOUR_TREE_SUCCEED_ON_AVERAGE_ANY_SCAN_HPP
#define BEHAVIOUR_TREE_SUCCEED_ON_AVERAGE_ANY_SCAN_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#include <spdlog/spdlog.h>
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

namespace behaviour_tree::node::custom::condition
{
    constexpr double CM_TO_DISTANCE = 15.151515151515151515151515151515151515151515151515151515;
    class SucceedOnAnyNearbyScan final : public CustomNode
    {
    public:
        SucceedOnAnyNearbyScan(const std::string &name, const int min_angle, const int max_angle, const double cm) : CustomNode(name),
                                                                                                                     min_angle(min_angle),
                                                                                                                     max_angle(max_angle),
                                                                                                                     cm(cm),
                                                                                                                     minimum_distance_unit(cm * CM_TO_DISTANCE)
        {
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