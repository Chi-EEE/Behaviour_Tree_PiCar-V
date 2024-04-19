#ifndef BEHAVIOUR_TREE_SUCCEED_ON_AVERAGE_COLOUR_HPP
#define BEHAVIOUR_TREE_SUCCEED_ON_AVERAGE_COLOUR_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

namespace behaviour_tree::node::custom::condition
{
    class SucceedOnAverageColour final : public CustomNode
    {
    public:
        SucceedOnAverageColour(const std::string &name, const std::string hex_colour, const double tolerance) : CustomNode(name),
                                                                                                          hex_colour(hex_colour),
                                                                                                          tolerance(tolerance)
        {
        }

        const static tl::expected<std::shared_ptr<SucceedOnAverageColour>, std::string> parse(const pugi::xml_node &node, const int index, const std::string &name_attribute)
        {
            std::string hex_colour = node.attribute("hex_colour").as_string();
            if (hex_colour.compare(0, 1, "#"))
            {
                hex_colour = "#" + hex_colour;
            }
            if (hex_colour.size() != 7)
            {
                return tl::unexpected(fmt::format(R"(Invalid hex_colour: '{}' | Condition:SucceedOnAverageColour:['{}',{}])", hex_colour, name_attribute, index));
            }
            for (int i = 1; i < hex_colour.size(); i++)
            {
                if (!std::isxdigit(hex_colour[i]))
                {
                    return tl::unexpected(fmt::format(R"(Invalid hex_colour: '{}' | Condition:SucceedOnAverageColour:['{}',{}])", hex_colour, name_attribute, index));
                }
            }
            const double tolerance = node.attribute("tolerance").as_double();
            if (tolerance < 0 || tolerance > 100)
            {
                return tl::unexpected(fmt::format(R"(Invalid tolerance: '{}' | Condition:SucceedOnAverageColour:['{}',{}])", tolerance, name_attribute, index));
            }
            return std::make_shared<SucceedOnAverageColour>(
                SucceedOnAverageColour(
                    name_attribute,
                    hex_colour,
                    tolerance));
        }

        const Status run(const int tick_count, std::shared_ptr<Context> context) final override
        {
            // Following made by ChatGPT
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
            std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
            auto car_system = car_context->getCarSystem();

            std::string frame_buffer = car_system->getDeviceManager()->getCameraDevice()->getFrameBuffer();
            std::vector<uchar> buffer(frame_buffer.begin(), frame_buffer.end());
            cv::Mat frame = cv::imdecode(buffer, cv::IMREAD_COLOR);
            if (frame.empty())
            {
                spdlog::error("Failed to decode frame buffer");
                return Status::Failure;
            }

            cv::Scalar avg_color = cv::mean(frame);
            std::string avg_color_hex = fmt::format("#{:02x}{:02x}{:02x}", static_cast<int>(avg_color[2]), static_cast<int>(avg_color[1]), static_cast<int>(avg_color[0]));

            int color_diff = calculateColorDifference(avg_color_hex, this->hex_colour);
            int max_color_diff = calculateMaxColorDifference(this->tolerance);

            if (color_diff <= max_color_diff)
            {
                return Status::Success;
            }
            else
            {
                return Status::Failure;
            }
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
            return Status::Failure;
        }

        // Following made by ChatGPT
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
        // Function to calculate the color difference between two hex colors
        int calculateColorDifference(const std::string &color1, const std::string &color2)
        {
            // Convert hex strings to RGB values
            cv::Scalar rgb_color1 = hexToRGB(color1);
            cv::Scalar rgb_color2 = hexToRGB(color2);

            // Calculate the Euclidean distance between the two colors in RGB space
            return static_cast<int>(cv::norm(rgb_color1, rgb_color2));
        }

        // Function to convert a hex color string to RGB values
        cv::Scalar hexToRGB(const std::string &hexColor)
        {
            std::string hex = hexColor.substr(1); // remove the '#' character
            int r, g, b;
            std::istringstream(hex.substr(0, 2)) >> std::hex >> r;
            std::istringstream(hex.substr(2, 2)) >> std::hex >> g;
            std::istringstream(hex.substr(4, 2)) >> std::hex >> b;
            return cv::Scalar(b, g, r);
        }

        // Function to calculate the maximum allowable color difference based on the percentage threshold
        int calculateMaxColorDifference(double percentage)
        {
            // Assuming percentage represents the acceptable deviation from the specified color
            // We can calculate the maximum allowable color difference based on this percentage
            // For example, if percentage is 10, it means 10% deviation is acceptable

            // Max color difference will be based on the maximum possible Euclidean distance between two colors in RGB space
            // For simplicity, let's assume the maximum distance is between pure black and pure white
            cv::Scalar black(0, 0, 0);
            cv::Scalar white(255, 255, 255);
            int max_color_diff = static_cast<int>(cv::norm(black, white));

            // Now, we scale the maximum color difference based on the percentage
            return static_cast<int>(max_color_diff * (percentage / 100.0));
        }
#endif // !BEHAVIOUR_TREE_DISABLE_RUNss

        const std::string getHexColour() const
        {
            return this->hex_colour;
        }

        const double getTolerance() const
        {
            return this->tolerance;
        }

        const std::string toString() const final override
        {
            const std::string &name = this->getName();
            if (name != "")
                return fmt::format(R"(<Condition:SucceedOnAverageColour name='{}' hex_colour='{}' tolerance='{}'/>)", name, this->getHexColour(), this->getTolerance());
            else
                return fmt::format(R"(<Condition:SucceedOnAverageColour hex_colour='{}' tolerance='{}'/>)", this->getHexColour(), this->getTolerance());
        }

    private:
        const std::string hex_colour;
        const double tolerance;
    };
}

#endif