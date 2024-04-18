#ifndef BEHAVIOUR_TREE_CHANGE_INTEGER_HPP
#define BEHAVIOUR_TREE_CHANGE_INTEGER_HPP

#pragma once

#include "BlackboardLeaf.hpp"

#include "enum/IntegerChangeType.hpp"

namespace behaviour_tree::node::blackboard
{
	class ChangeInteger final : public BlackboardLeaf
	{
	public:
		ChangeInteger(const std::string& name) : BlackboardLeaf(name) {}

		const Status run(const int tick_count, std::shared_ptr<Context> context) final override
		{
			if (!context->containsBlackboard(this->variable_name)) {
				context->setBlackboard(this->variable_name, 0);
			}
			int value = context->getBlackboard(this->variable_name);
			switch (this->integer_change_type) {
			case IntegerChangeType::Set: {
				context->setBlackboard(this->variable_name, this->value);
				break;
			}
			case IntegerChangeType::Add: {
				context->setBlackboard(this->variable_name, value + this->value);
				break;
			}
			case IntegerChangeType::Subtract: {
				context->setBlackboard(this->variable_name, value - this->value);
				break;
			}
			}
			return Status::Success;
		}

		const std::string toString() const final override
		{
			const std::string& name = this->getName();
			const std::string integer_change_type = [&]() {
				switch (this->integer_change_type)
				{
				case IntegerChangeType::Set:
					return "Set";
				case IntegerChangeType::Add:
					return "Add";
				case IntegerChangeType::Subtract:
					return "Subtract";
				default:
					return "Invalid";
				}
				}
			();
			if (name != "")
				return fmt::format(R"(<Blackboard:ChangeInteger name='{}' variable_name='{}' value='{}' integer_change_type='{}'/>)", name, this->variable_name, this->value, integer_change_type);
			else
				return fmt::format(R"(<Blackboard:ChangeInteger variable_name='{}' value='{}' integer_change_type='{}'/>)", this->variable_name, this->value, integer_change_type);
		}

	private:
		const std::string variable_name;
		const IntegerChangeType integer_change_type;
		const int value;
	};
}

#endif