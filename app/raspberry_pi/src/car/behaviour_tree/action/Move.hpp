#ifndef BEHAVIOUR_TREE_MOVE_HPP
#define BEHAVIOUR_TREE_MOVE_HPP

#pragma once

#include "behaviour_tree/action/Action.hpp"

namespace behaviour_tree::action
{
	class Move : public Action
	{
	public:
		Move(const int& ms) : ms(ms)
		{
		}

		const Status run(Context& context) override {
			/*using namespace car::behaviour_tree;
			CarContext &car_context = static_cast<CarContext&>(context);*/
			return Status::Success;
		}

		const std::string toString() const override {
			return fmt::format(R"(<Action type="Move" ms="{}"/>)", this->getMS());
		}

		const int& getMS() const {
			return this->ms;
		}

	private:
		const int ms;
	};
}

#endif