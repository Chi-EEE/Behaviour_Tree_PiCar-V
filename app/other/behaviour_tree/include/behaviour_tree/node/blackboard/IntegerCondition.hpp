#ifndef BEHAVIOUR_TREE_INTEGER_CONDITION_HPP
#define BEHAVIOUR_TREE_INTEGER_CONDITION_HPP

#pragma once

#include "BlackboardNode.hpp"

#include "enum/ConditionOperatorType.hpp"

namespace behaviour_tree::node::blackboard
{
	class IntegerCondition final : public BlackboardNode
	{
	public:
		IntegerCondition(const std::string& name, const std::string variable_name, const ConditionOperatorType condition_operator, const int value) : BlackboardNode(name), variable_name(variable_name), condition_operator(condition_operator), value(value) {}

		const BlackboardType type() const final override { return BlackboardType::IntegerCondition; }

		const Status run(const int tick_count, std::shared_ptr<Context> context) final override
		{
			if (!context->containsBlackboard(this->variable_name)) {
				context->setBlackboard(this->variable_name, 0);
			}
			switch (this->condition_operator)
			{
			case ConditionOperatorType::Equal:
				return context->getBlackboard(this->variable_name) == this->value ? Status::Success : Status::Failure;
			case ConditionOperatorType::GreaterThan:
				return context->getBlackboard(this->variable_name) > this->value ? Status::Success : Status::Failure;
			case ConditionOperatorType::GreaterThanOrEqual:
				return context->getBlackboard(this->variable_name) >= this->value ? Status::Success : Status::Failure;
			case ConditionOperatorType::LessThan:
				return context->getBlackboard(this->variable_name) < this->value ? Status::Success : Status::Failure;
			case ConditionOperatorType::LessThanOrEqual:
				return context->getBlackboard(this->variable_name) <= this->value ? Status::Success : Status::Failure;
			case ConditionOperatorType::NotEqual:
				return context->getBlackboard(this->variable_name) != this->value ? Status::Success : Status::Failure;
			default:
				return Status::Failure;
			}
		}

		const std::string toString() const final override
		{
			const std::string& name = this->getName();
			const std::string condition_operator = [&]() {
				switch (this->condition_operator)
				{
				case ConditionOperatorType::Equal:
					return "=";
				case ConditionOperatorType::GreaterThan:
					return ">";
				case ConditionOperatorType::GreaterThanOrEqual:
					return ">=";
				case ConditionOperatorType::LessThan:
					return "<";
				case ConditionOperatorType::LessThanOrEqual:
					return "<=";
				case ConditionOperatorType::NotEqual:
					return "!=";
				default:
					return "Invalid";
				}
				}
			();
			if (name != "")
				return fmt::format(R"(<Blackboard:IntegerCondition name='{}' variable_name='{}' condition_operator='{}' value='{}'/>)", name, this->variable_name, condition_operator, this->value);
			else
				return fmt::format(R"(<Blackboard:IntegerCondition variable_name='{}' condition_operator='{}' value='{}'/>)", this->variable_name, condition_operator, this->value);
		}

	private:
		const std::string variable_name;
		const ConditionOperatorType condition_operator;
		const int value;
	};
}

#endif