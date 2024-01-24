#ifndef BEHAVIOUR_TREE_ERROR_HPP
#define BEHAVIOUR_TREE_ERROR_HPP

#pragma once

#include "behaviour_tree/node/custom/Action.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

namespace behaviour_tree::node::custom::action
{
    class Error : public Action
    {
    public:
        Error(const std::string& name, const std::string& text) : Action(name), text(text)
        {
        }

        const Status tick(const int& tick_count, std::shared_ptr<Context> context) override
        {
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
            std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
            car_context->error(this->getText());
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
        }

        const std::string& getText() const {
            return this->text;
        }

        const std::string toString() const override {
            const std::string& name = this->getName();
            if (name != "")
                return fmt::format(R"(<Action:Error name="{}" text="{}"/>)", name, this->getText());
            else
                return fmt::format(R"(<Action:Error text="{}"/>)", this->getText());
        }

    private:
        const std::string text;
    };
}

#endif