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
        ChangeInteger(const std::string &name) : BlackboardLeaf(name) {}

        const Status run(const int tick_count, std::shared_ptr<Context> context) final override
        {
        }

        const std::string toString() const final override
        {
            const std::string &name = this->getName();
            if (name != "")
                return fmt::format(R"(<Blackboard:ChangeInteger name='{}' type='{}'/>)", name, out);
            else
                return fmt::format(R"(<Blackboard:ChangeInteger type='{}'/>)", out);
        }

    private:
        std::string variable_name;
        int value;
    };
}

#endif