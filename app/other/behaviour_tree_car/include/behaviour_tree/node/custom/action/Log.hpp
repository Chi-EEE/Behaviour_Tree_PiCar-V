#ifndef BEHAVIOUR_TREE_LOG_HPP
#define BEHAVIOUR_TREE_LOG_HPP

#pragma once

#include "behaviour_tree/node/custom/Action.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

namespace behaviour_tree::node::custom::action
{
    class Log : public Action
    {
    public:
        Log(const std::string& name, const std::string& text) : Action(name), text(text)
        {
        }

        const Status tick(Context& context) override
        {
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			CarContext& car_context = static_cast<CarContext&>(context);
            car_context.log(this->getText());
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
            return Status::Success;
        }

        const std::string& getText() const {
            return this->text;
        }

        const std::string toString() const override {
            const std::string& name = this->getName();
            if (name != "")
                return fmt::format(R"(<Action:Log name="{}" text="{}"/>)", name, this->getText());
            else
                return fmt::format(R"(<Action:Log text="{}"/>)", this->getText());
        }

    private:
        const std::string text;
    };
}

#endif