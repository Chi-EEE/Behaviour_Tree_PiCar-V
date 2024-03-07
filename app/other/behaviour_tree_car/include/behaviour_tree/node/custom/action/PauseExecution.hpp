#ifndef BEHAVIOUR_TREE_PAUSE_EXECUTION_HPP
#define BEHAVIOUR_TREE_PAUSE_EXECUTION_HPP

#pragma once

#include <chrono>

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"

namespace behaviour_tree::node::custom::action
{
	class PauseExecution final : public CustomNode
	{
	public:
		PauseExecution(const std::string &name, const int &ms) : CustomNode(name), ms(ms)
		{
		}

		void start(std::shared_ptr<Context> context) final override {
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			this->start_time = std::chrono::steady_clock::now();
#endif
		}

		void finish(std::shared_ptr<Context> context) final override {
		}

		const Status run(const int &tick_count, std::shared_ptr<Context> context) final override
		{
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->start_time).count() < this->ms)
			{
				return Status::Running;
			}
#endif
			return Status::Success;
		}

		const std::string toString() const final override
		{
			const std::string &name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:PauseExecution name='{}' ms='{}'/>)", name, this->getMS());
			else
				return fmt::format(R"(<Action:PauseExecution ms='{}'/>)", this->getMS());
		}

		const int &getMS() const
		{
			return this->ms;
		}

	private:
		std::chrono::steady_clock::time_point start_time;
		const int ms;
	};
}

#endif