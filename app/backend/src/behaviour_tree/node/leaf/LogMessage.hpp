#ifndef BEHAVIOUR_TREE_LOGMESSAGE_HPP
#define BEHAVIOUR_TREE_LOGMESSAGE_HPP

#pragma once

#include "Leaf.hpp"

namespace behaviour_tree::node::leaf
{
    class LogMessage : public Leaf
    {
    public:
        LogMessage(const std::string& name, const std::string& text) : Leaf(name), text(text)
        {
        }

        const LeafType type() const override { return LeafType::LogMessage; }

        const Status run() override
        {
            // std::cout << text << std::endl;
            return Status::SUCCESS;
        }

        const std::string& getText() const {
            return this->text;
        }

        const std::string toString() const override {
            return fmt::format(R"(<LogMessage name="{}" text="{}"/>)", this->getName(), this->getText());
        }

    private:
        const std::string text;
    };
}

#endif