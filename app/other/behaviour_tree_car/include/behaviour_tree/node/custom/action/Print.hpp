#ifndef BEHAVIOUR_TREE_PRINT_HPP
#define BEHAVIOUR_TREE_PRINT_HPP

#pragma once

#include <iostream>

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

namespace behaviour_tree::node::custom::action
{
	class Print final : public CustomNode
	{
	public:
		Print(const std::string& name, const std::string& text) : CustomNode(name), text(text)
		{
		}

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override
		{
// #ifndef BEHAVIOUR_TREE_DISABLE_RUN
// 			std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
// #else
// 			std::cout << this->getText();
// #endif
			std::cout << this->getText();
			return Status::Success;
		}

		const std::string& getText() const {
			return this->text;
		}

		const std::string toString() const final override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:Print name='{}' text='{}'/>)", name, this->getText());
			else
				return fmt::format(R"(<Action:Print text='{}'/>)", this->getText());
		}

	private:
		const std::string text;
	};
}

#endif