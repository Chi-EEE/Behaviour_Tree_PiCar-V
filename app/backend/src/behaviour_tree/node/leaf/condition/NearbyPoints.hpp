#ifndef BEHAVIOUR_TREE_NEARBY_POINTS_HPP
#define BEHAVIOUR_TREE_NEARBY_POINTS_HPP

#pragma once

#include "../Condition.hpp"

namespace behaviour_tree::node::leaf::condition
{
	class NearbyPoints : public Condition
	{
	public:
		NearbyPoints(const std::string& name, const int& min_angle, const int& max_angle, const int& avg_distance) : Condition(name),
			min_angle(min_angle),
			max_angle(max_angle),
			avg_distance(avg_distance)
		{
		}

		const ConditionType condition_type() const override { return ConditionType::NearbyPoints; }

		const std::string toString() const override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Condition name="{}" type="NearbyPoints" min_angle="{}" max_angle="{}" avg_distance="{}"/>)", name, this->getMinAngle(), this->getMaxAngle(), this->getAvgDistance());
			else
				return fmt::format(R"(<Condition type="NearbyPoints" min_angle="{}" max_angle="{}" avg_distance="{}"/>)", this->getMinAngle(), this->getMaxAngle(), this->getAvgDistance());
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

	private:
		const int min_angle;
		const int max_angle;

		const int avg_distance;
	};
}

#endif