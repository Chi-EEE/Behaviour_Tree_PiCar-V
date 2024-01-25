#ifndef BEHAVIOUR_TREE_WAIT_HPP
#define BEHAVIOUR_TREE_WAIT_HPP

#pragma once

#include <chrono>

#include "behaviour_tree/node/custom/Action.hpp"

#include "behaviour_tree/Context.h"

namespace behaviour_tree::node::custom::action
{
	class Wait final : public Action
	{
	public:
		Wait(const std::string& name, const int& ms, const bool& reset_on_non_consecutive_tick) : Action(name), ms(ms), reset_on_non_consecutive_tick(reset_on_non_consecutive_tick)
		{
		}

		const Status tick(const int& tick_count, std::shared_ptr<Context> context) final override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			if (this->reset_on_non_consecutive_tick && this->previous_tick_count != tick_count && this->previous_tick_count + 1 != tick_count) {
				this->start = std::chrono::steady_clock::now();
			}
			this->previous_tick_count = tick_count;
			if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->start).count() < this->ms)
			{
				// Reset the previous tick count so that the wait will be reset if the node is ticked again
				this->previous_tick_count = -1;
				return Status::Running;
			}
#endif
			return Status::Success;
		}

		const std::string toString() const final override {
			const std::string& name = this->getName();
			const std::string& reset_on_non_consecutive_tick_string = this->getResetOnNonConsecutiveTick() ? "true" : "false";
			if (name != "")
				return fmt::format(R"(<Action:Wait name="{}" ms="{}" reset_on_non_consecutive_tick="{}"/>)", name, this->getMS(), reset_on_non_consecutive_tick_string);
			else
				return fmt::format(R"(<Action:Wait ms="{}" reset_on_non_consecutive_tick="{}"/>)", this->getMS(), reset_on_non_consecutive_tick_string);
		}

		const int& getMS() const {
			return this->ms;
		}

		const bool& getResetOnNonConsecutiveTick() const {
			return this->reset_on_non_consecutive_tick;
		}

	private:
		std::chrono::steady_clock::time_point start;
		int previous_tick_count = -1;
		const int ms;
		const bool reset_on_non_consecutive_tick;
	};
}

#endif