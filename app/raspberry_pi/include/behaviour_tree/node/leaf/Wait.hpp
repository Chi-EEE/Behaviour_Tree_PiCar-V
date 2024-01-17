#ifndef BEHAVIOUR_TREE_WAIT_HPP
#define BEHAVIOUR_TREE_WAIT_HPP

#pragma once

#include <thread>

#include "Leaf.hpp"

namespace behaviour_tree::node::leaf
{
	class Wait : public Leaf
	{
	public:
		Wait(const std::string& name, const int& ms) : Leaf(name), ms(ms)
		{
		}

		const LeafType type() const override { return LeafType::Wait; }

		const Status run(Context& context) override
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(this->ms));
			return Status::Success;
		}

		const std::string toString() const override {
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Wait name="{}" ms="{}"/>)", name, this->getMS());
			else
				return fmt::format(R"(<Wait ms="{}"/>)", this->getMS());
		}

		const int& getMS() const {
			return this->ms;
		}

	private:
		const int ms;
	};
}

#endif