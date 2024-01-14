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

		const Status run()
		{
			// TODO:
			return Status::SUCCESS;
		}

	private:
		const int min_angle;
		const int max_angle;

		const int avg_distance;
	};
}

#endif