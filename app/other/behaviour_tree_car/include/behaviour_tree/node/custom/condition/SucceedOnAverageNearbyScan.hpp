#ifndef BEHAVIOUR_TREE_SUCCEED_ON_AVERAGE_NEARBY_SCAN_HPP
#define BEHAVIOUR_TREE_SUCCEED_ON_AVERAGE_NEARBY_SCAN_HPP

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
    class SucceedOnAverageNearbyScan final : public CustomNode
    {
    public:
        SucceedOnAverageNearbyScan(const std::string &name, const int min_angle, const int max_angle, const double cm, const int smallest_measure_amount_used) : CustomNode(name),
                                                                                                                                                                 min_angle(min_angle),
                                                                                                                                                                 max_angle(max_angle),
                                                                                                                                                                 cm(cm),
                                                                                                                                                                 smallest_measure_amount_used(smallest_measure_amount_used),
                                                                                                                                                                 average_distance_unit(cm * global::CM_TO_DISTANCE)
        {
        }

        const static tl::expected<std::shared_ptr<SucceedOnAverageNearbyScan>, std::string> parse(const pugi::xml_node &node, const int index, const std::string &name_attribute)
        {
            const int min_angle = node.attribute("min_angle").as_int();
            if (min_angle < 0 || min_angle > 360)
            {
                return tl::unexpected(fmt::format(R"(Invalid min_angle: '{}' | Condition:SucceedOnAverageNearbyScan:['{}',{}])", min_angle, name_attribute, index));
            }
            const int max_angle = node.attribute("max_angle").as_int();
            if (max_angle < 0 || max_angle > 360)
            {
                return tl::unexpected(fmt::format(R"(Invalid max_angle: '{}' | Condition:SucceedOnAverageNearbyScan:['{}',{}])", max_angle, name_attribute, index));
            }
            const double cm = node.attribute("cm").as_double();
            if (cm < 0)
            {
                return tl::unexpected(fmt::format(R"(Invalid cm: '{}' | Condition:SucceedOnAverageNearbyScan:['{}',{}])", cm, name_attribute, index));
            }
            const int minimum_measure_amount_used = node.attribute("minimum_measure_amount_used").as_int();
            if (minimum_measure_amount_used < 0)
            {
                return tl::unexpected(fmt::format(R"(Invalid minimum_measure_amount_used: '{}' | Condition:SucceedOnAverageNearbyScan:['{}',{}])", minimum_measure_amount_used, name_attribute, index));
            }
            return std::make_shared<SucceedOnAverageNearbyScan>(
                SucceedOnAverageNearbyScan(
                    name_attribute,
                    min_angle,
                    max_angle,
                    cm,
                    minimum_measure_amount_used));
        }

        const Status run(const int tick_count, std::shared_ptr<Context> context) final override
        {
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
            std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
            auto car_system = car_context->getCarSystem();
            double total_distance = 0.0;
            int angles_between_count = 0;
            std::vector<Measure> scan_data = car_system->getDeviceManager()->getLidarDevice()->getScanData();
            if (this->smallest_measure_amount_used > 0)
            {
                std::vector<Measure> smallest_measures;
                std::partial_sort(scan_data.begin(), scan_data.begin() + this->smallest_measure_amount_used, scan_data.end(),
                                  [](const Measure &a, const Measure &b)
                                  { return a.distance < b.distance; });
                smallest_measures.insert(smallest_measures.end(), scan_data.begin(), scan_data.begin() + this->smallest_measure_amount_used);
                scan_data = smallest_measures;
            }
            for (auto &measure : scan_data)
            {
                if (measure.angle > this->getMinAngle() && measure.angle < this->getMaxAngle())
                {
                    total_distance += measure.distance;
                    ++angles_between_count;
                }
            }
            if (angles_between_count > 0)
            {
                double average_distance_unit = total_distance / angles_between_count;
                if (average_distance_unit < this->getAverageDistanceUnit())
                {
                    return Status::Success;
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

        const double getAverageDistanceUnit() const
        {
            return this->average_distance_unit;
        }

        const double getCentimeters() const
        {
            return this->cm;
        }

        const int getSmallestMeasureAmountUsed() const
        {
            return this->smallest_measure_amount_used;
        }

        const std::string toString() const final override
        {
            const std::string &name = this->getName();
            if (name != "")
                return fmt::format(R"(<Condition:SucceedOnAverageNearbyScan name='{}' min_angle='{}' max_angle='{}' cm='{}' smallest_measure_amount_used='{}'/>)", name, this->getMinAngle(), this->getMaxAngle(), this->getCentimeters(), this->getSmallestMeasureAmountUsed());
            else
                return fmt::format(R"(<Condition:SucceedOnAverageNearbyScan min_angle='{}' max_angle='{}' cm='{}' smallest_measure_amount_used='{}'/>)", this->getMinAngle(), this->getMaxAngle(), this->getCentimeters(), this->getSmallestMeasureAmountUsed());
        }

    private:
        const int min_angle;
        const int max_angle;

        const double cm;

        const int smallest_measure_amount_used;

        const double average_distance_unit;
    };
}

#endif