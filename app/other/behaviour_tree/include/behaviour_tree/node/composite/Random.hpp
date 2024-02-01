#ifndef BEHAVIOUR_TREE_RANDOM_HPP
#define BEHAVIOUR_TREE_RANDOM_HPP

#pragma once

#include <effolkronium/random.hpp>

#include "Composite.hpp"

using effolkronium_Random = effolkronium::random_static;

namespace behaviour_tree::node::composite
{
	class Random final : public Composite
	{
	public:
		Random(const std::string& name, std::vector<std::shared_ptr<Node>> children) : Composite(name, std::move(children)) {}

		const CompositeType type() const final override { return CompositeType::Random; }

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override
		{
			return this->run(tick_count, context, 0);
		}

		const Status run(const int& tick_count, std::shared_ptr<Context> context, const int& start_index) final override
		{
			// Random can never be empty from the BehaviourTreeParser
			return this->children[effolkronium_Random::get(0, static_cast<int>(this->children.size() - 1))]->tick(tick_count, context);
		}

		const std::string toString() const final override {
			std::string out;
			for (auto& child : this->children)
			{
				out += child->toString();
			}
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Random name="{}">{}</Random>)", name, out);
			else
				return fmt::format(R"(<Random>{}</Random>)", out);
		}
	};
}

#endif