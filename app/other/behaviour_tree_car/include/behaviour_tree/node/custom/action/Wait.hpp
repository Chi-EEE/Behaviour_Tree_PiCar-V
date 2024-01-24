#ifndef BEHAVIOUR_TREE_WAIT_HPP
#define BEHAVIOUR_TREE_WAIT_HPP

#pragma once

#include "behaviour_tree/node/custom/Action.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

namespace behaviour_tree::node::custom::action
{
	class Wait : public Action
	{
	public:
		Wait(const std::string& name, const int& ms) : Action(name), ms(ms)
		{
		}

		const Status run(Context& context) override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			CarContext& car_context = static_cast<CarContext&>(context);
			auto car_system = car_context.getCarSystem();
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
		}

		const std::string toString() const override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:Wait name="{}" ms="{}"/>)", name, this->getMS());
			else
				return fmt::format(R"(<Action:Wait ms="{}"/>)", this->getMS());
		}

		const int& getMS() const {
			return this->ms;
		}

	private:
		const int ms;
	};
}

#endif