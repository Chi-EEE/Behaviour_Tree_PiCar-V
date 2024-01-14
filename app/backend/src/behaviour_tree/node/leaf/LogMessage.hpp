#ifndef BEHAVIOUR_TREE_LOGMESSAGE_HPP
#define BEHAVIOUR_TREE_LOGMESSAGE_HPP

#pragma once

#include "Leaf.hpp"

namespace behaviour_tree::node::leaf
{
    class LogMessage : public Leaf
    {
    public:
        LogMessage(std::string name, std::string text) : Leaf(name), text(text)
        {
        }

        const LeafType type() const override { return LeafType::LogMessage; }

        const Status run() override
        {
            // std::cout << text << std::endl;
            return Status::SUCCESS;
        }

    private:
        std::string text;
    };
}

#endif