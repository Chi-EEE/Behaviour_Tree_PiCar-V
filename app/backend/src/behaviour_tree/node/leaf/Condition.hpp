#ifndef BEHAVIOUR_TREE_CONDITION_HPP
#define BEHAVIOUR_TREE_CONDITION_HPP

#pragma once

#include "Leaf.hpp"

#include "condition/ConditionType.hpp"

namespace behaviour_tree::node::leaf
{
	class Condition : public Leaf
	{
	public:
		Condition(const std::string& name) : Leaf(std::move(name)) {}

		const LeafType type() const override { return LeafType::Condition; }

		virtual const condition::ConditionType condition_type() const = 0;
	};
}

#endif