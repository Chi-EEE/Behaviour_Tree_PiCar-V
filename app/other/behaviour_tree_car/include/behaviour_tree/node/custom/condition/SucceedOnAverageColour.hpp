#ifndef BEHAVIOUR_TREE_SUCCEED_ON_AVERAGE_COLOUR_HPP
#define BEHAVIOUR_TREE_SUCCEED_ON_AVERAGE_COLOUR_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#include <spdlog/spdlog.h>
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

namespace behaviour_tree::node::custom::condition
{
    class SucceedOnAverageColour final : public CustomNode
    {
    public:
        SucceedOnAverageColour(const std::string &name, const std::string hex, const double percentage) : CustomNode(name),
                                                                                                          hex(hex),
                                                                                                          percentage(percentage)
        {
        }

        const static tl::expected<std::shared_ptr<SucceedOnAverageColour>, std::string> parse(const pugi::xml_node &node, const int index, const std::string &name_attribute)
        {
            const std::string hex = node.attribute("hex").as_string();
            const double percentage = node.attribute("percentage").as_double();
            if (percentage < 0 || percentage > 100)
            {
                return tl::unexpected(fmt::format(R"(Invalid percentage: '{}' | Condition:SucceedOnAverageColour:['{}',{}])", percentage, name_attribute, index));
            }
            return std::make_shared<SucceedOnAverageColour>(
                SucceedOnAverageColour(
                    name_attribute,
                    hex,
                    percentage));
        }

        const Status run(const int tick_count, std::shared_ptr<Context> context) final override
        {
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
            std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
            auto car_system = car_context->getCarSystem();
            std::string frame_buffer = car_system->getDeviceManager()->getCameraDevice()->getFrameBuffer();
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
            return Status::Failure;
        }

        const std::string getHex() const
        {
            return this->hex;
        }

        const double getPercentage() const
        {
            return this->percentage;
        }

        const std::string toString() const final override
        {
            const std::string &name = this->getName();
            if (name != "")
                return fmt::format(R"(<Condition:SucceedOnAverageColour name='{}' hex='{}' percentage='{}'/>)", name, this->getHex(), this->getPercentage());
            else
                return fmt::format(R"(<Condition:SucceedOnAverageColour hex='{}' percentage='{}'/>)", this->getHex(), this->getPercentage());
        }

    private:
        const std::string hex;
        const double percentage;
    };
}

#endif