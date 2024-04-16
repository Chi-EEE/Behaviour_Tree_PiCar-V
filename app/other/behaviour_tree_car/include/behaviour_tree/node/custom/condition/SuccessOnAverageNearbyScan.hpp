#ifndef BEHAVIOUR_TREE_SUCCESS_ON_AVERAGE_NEARBY_SCAN_HPP
#define BEHAVIOUR_TREE_SUCCESS_ON_AVERAGE_NEARBY_SCAN_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#include <spdlog/spdlog.h>
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

namespace behaviour_tree::node::custom::condition
{
	constexpr double CM_TO_DISTANCE = 6.666666666666666666666666666666666666666666666;
	class SuccessOnAverageNearbyScan final : public CustomNode
	{
	public:
		SuccessOnAverageNearbyScan(const std::string& name, const int min_angle, const int max_angle, const double& cm) :
			CustomNode(name),
			min_angle(min_angle),
			max_angle(max_angle),
			cm(cm),
			average_distance_unit(cm * CM_TO_DISTANCE)
		{
		}

		const Status run(const int tick_count, std::shared_ptr<Context> context) final override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
			auto car_system = car_context->getCarSystem();
			double total_distance = 0.0;
			int angles_between_count = 0;
			for (auto &measure : car_system->getDeviceManager()->getLidarDevice()->getScanData())
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
                spdlog::info("Average Distance: {}; Average Distance Unit: {}", average_distance_unit, this->getAverageDistanceUnit());
				if (average_distance_unit < this->getAverageDistanceUnit())
				{
					return Status::Success;
				}
			}
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Failure;
		}

		const int getMinAngle() const {
			return this->min_angle;
		}

		const int getMaxAngle() const {
			return this->max_angle;
		}

		const double& getAverageDistanceUnit() const {
			return this->average_distance_unit;
		}

		const double& getCM() const {
			return this->cm;
		}

		const std::string toString() const final override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Condition:SuccessOnAverageNearbyScan name='{}' min_angle='{}' max_angle='{}' cm='{}'/>)", name, this->getMinAngle(), this->getMaxAngle(), this->getCM());
			else
				return fmt::format(R"(<Condition:SuccessOnAverageNearbyScan min_angle='{}' max_angle='{}' cm='{}'/>)", this->getMinAngle(), this->getMaxAngle(), this->getCM());
		}

	private:
		const int min_angle;
		const int max_angle;

		const double cm;
		const double average_distance_unit;
	};
}

#endif