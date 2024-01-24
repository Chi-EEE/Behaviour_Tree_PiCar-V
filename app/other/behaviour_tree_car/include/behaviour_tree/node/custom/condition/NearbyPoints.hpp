#ifndef BEHAVIOUR_TREE_NEARBY_POINTS_HPP
#define BEHAVIOUR_TREE_NEARBY_POINTS_HPP

#pragma once

#include "behaviour_tree/node/custom/Condition.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

namespace behaviour_tree::node::custom::condition
{
	class NearbyPoints final : public Condition
	{
	public:
		NearbyPoints(const std::string& name, const int& min_angle, const int& max_angle, const int& avg_distance) :
			Condition(name),
			min_angle(min_angle),
			max_angle(max_angle),
			avg_distance(avg_distance)
		{
		}

		const Status tick(const int& tick_count, std::shared_ptr<Context> context) final override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
			auto car_system = car_context->getCarSystem();
			for (auto& measure : car_system->getScanData()) {
				if (measure.angle > this->getMinAngle() && measure.angle < this->getMaxAngle()) {
					if (measure.distance < this->getAvgDistance()) {
						return Status::Failure;
					}
				}
			}
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
		}

		const int& getMinAngle() const {
			return this->min_angle;
		}

		const int& getMaxAngle() const {
			return this->max_angle;
		}

		const int& getAvgDistance() const {
			return this->avg_distance;
		}

		const std::string toString() const final override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Condition:NearbyPoints name="{}" min_angle="{}" max_angle="{}" avg_distance="{}"/>)", name, this->getMinAngle(), this->getMaxAngle(), this->getAvgDistance());
			else
				return fmt::format(R"(<Condition:NearbyPoints min_angle="{}" max_angle="{}" avg_distance="{}"/>)", this->getMinAngle(), this->getMaxAngle(), this->getAvgDistance());
		}

	private:
		const int min_angle;
		const int max_angle;

		const int avg_distance;
	};
}

#endif