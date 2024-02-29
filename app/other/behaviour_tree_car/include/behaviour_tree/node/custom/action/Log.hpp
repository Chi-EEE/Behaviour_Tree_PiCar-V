#ifndef BEHAVIOUR_TREE_LOG_HPP
#define BEHAVIOUR_TREE_LOG_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

namespace behaviour_tree::node::custom::action
{
	class Log final : public CustomNode
	{
	public:
		Log(const std::string& name, const std::string& text) : CustomNode(name), text(text)
		{
		}

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
			car_context->log(this->getText());
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
		}

		const std::string& getText() const {
			return this->text;
		}

		const std::string toString() const final override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:Log name='{}' text='{}'/>)", name, this->getText());
			else
				return fmt::format(R"(<Action:Log text='{}'/>)", this->getText());
		}

	private:
		const std::string text;
	};
}

#endif