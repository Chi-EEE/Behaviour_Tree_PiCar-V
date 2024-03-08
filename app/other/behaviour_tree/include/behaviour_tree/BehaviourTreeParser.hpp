#ifndef BEHAVIOUR_TREE_PARSER_HPP
#define BEHAVIOUR_TREE_PARSER_HPP

#pragma once

#include <filesystem>
#include <string>
#include <numeric>

#include <tl/expected.hpp>
#include <pugixml.hpp>
#include <fmt/format.h>

#include "utils/Utility.hpp"

#include "BehaviourTree.hpp"
#include "Root.hpp"
#include "node/Node.hpp"

#include "node/composite/Composite.hpp"
#include "node/composite/Sequence.hpp"
#include "node/composite/Selector.hpp"
#include "node/composite/Random.hpp"

#include "node/decorator/Decorator.hpp"
#include "node/decorator/Repeat.hpp"
#include "node/decorator/Invert.hpp"

#include "node/leaf/Leaf.hpp"
#include "node/leaf/Succeed.hpp"
#include "node/leaf/Fail.hpp"
#include "node/leaf/UseRoot.hpp"

#include "node/custom/CustomNode.hpp"

#include "node/custom/CustomNodeParser.hpp"

using namespace behaviour_tree::node;
using namespace behaviour_tree::node::composite;
using namespace behaviour_tree::node::decorator;
using namespace behaviour_tree::node::leaf;
using namespace behaviour_tree::node::custom;

namespace behaviour_tree
{
    static constexpr int STARTING_INDEX = 1;

    class BehaviourTreeParser
    {
    public:
        BehaviourTreeParser(BehaviourTreeParser const &) = delete;
        void operator=(BehaviourTreeParser const &) = delete;

        static BehaviourTreeParser &instance()
        {
            static BehaviourTreeParser instance;
            return instance;
        }

        void setCustomNodeParser(std::shared_ptr<CustomNodeParser> custom_node_parser)
        {
            this->custom_node_parser = std::move(custom_node_parser);
        }

        tl::expected<std::shared_ptr<BehaviourTree>, std::string> parseXML(const std::string &xml)
        {
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_string(xml.c_str());
            if (!result)
            {
                return tl::unexpected("XML parse error: " + std::string(result.description()));
            }
            return parse(doc);
        }

        tl::expected<std::shared_ptr<BehaviourTree>, std::string> parseFileXML(const std::string &file_path)
        {
            if (std::filesystem::exists(file_path) == false)
            {
                return tl::unexpected("File [" + file_path + "] does not exist");
            }
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file(file_path.c_str());
            if (!result)
            {
                return tl::unexpected("XML [" + file_path + "] parsed with errors");
            }
            return parse(doc);
        }

    private:
        BehaviourTreeParser() {}

        std::shared_ptr<CustomNodeParser> custom_node_parser;

        tl::expected<std::shared_ptr<BehaviourTree>, std::string> parse(pugi::xml_document &doc)
        {
            pugi::xml_node behaviour_tree_node = doc.child("BehaviourTree");
            if (!behaviour_tree_node)
            {
                return tl::unexpected(R"(No "BehaviourTree" node found)");
            }
            std::vector<std::shared_ptr<Root>> roots;
            for (pugi::xml_node node = behaviour_tree_node.child("Root"); node; node = node.next_sibling("Root"))
            {
                auto maybe_root = parseRoot(node, STARTING_INDEX + roots.size());
                if (!maybe_root.has_value())
                {
                    return tl::unexpected(maybe_root.error());
                }
                roots.push_back(std::move(maybe_root.value()));
            }
            if (roots.size() <= 0)
            {
                return tl::unexpected(R"(No "Root" nodes found in BehaviourTree)");
            }
            return std::make_shared<BehaviourTree>(
                BehaviourTree(behaviour_tree_node.attribute("repeat").as_bool(true), std::move(roots)));
        }

    private:
        tl::expected<std::shared_ptr<Root>, std::string> parseRoot(const pugi::xml_node &node, const int &index)
        {
            int child_count = 0;
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
                ++child_count;
            const std::string id = node.attribute("id").as_string();
            if (child_count == 0)
            {
                return std::make_shared<Root>(
                    Root(id, nullptr));
            }
            if (child_count != 1)
            {
                return tl::unexpected(fmt::format(R"(Root node must have only at most one child | Root:['{}',{}])", id, index));
            }
            const pugi::xml_node child = node.first_child();
            auto maybe_child_node = parseChild(child, STARTING_INDEX);
            if (!maybe_child_node.has_value())
            {
                return tl::unexpected(fmt::format(R"({}<-Root:['{}',{}])", maybe_child_node.error(), id, index));
            }
            return std::make_shared<Root>(
                Root(id, std::move(maybe_child_node.value())));
        }

        tl::expected<std::shared_ptr<node::Node>, std::string> parseChild(const pugi::xml_node &node, const int &index)
        {
            const std::string name_attribute = node.attribute("name").as_string();
            const std::string &node_name = node.name();
            switch (utils::hash(node_name))
            {
#pragma region Composite Node
            case utils::hash("Sequence"):
            {
                return parseComposite(node, index, CompositeType::Sequence);
            }
            case utils::hash("Selector"):
            {
                return parseComposite(node, index, CompositeType::Selector);
            }
            case utils::hash("Random"):
            {
                return parseComposite(node, index, CompositeType::Random);
            }
#pragma endregion
#pragma region Decorator Node
            case utils::hash("Repeat"):
            {
                return parseDecorator(node, index, DecoratorType::Repeat);
            }
            case utils::hash("Invert"):
            {
                return parseDecorator(node, index, DecoratorType::Invert);
            }
#pragma endregion
#pragma region Leaf Node
            case utils::hash("Succeed"):
            {
                return std::make_shared<Succeed>(Succeed(name_attribute));
            }
            case utils::hash("Fail"):
            {
                return std::make_shared<Fail>(Fail(name_attribute));
            }
            case utils::hash("UseRoot"):
            {
                return std::make_shared<UseRoot>(
                    UseRoot(
                        name_attribute,
                        node.attribute("id").as_string()));
            }
#pragma endregion
            default:
            {
                return this->parseCustomNode(node, index);
            }
            }
        }

        tl::expected<std::shared_ptr<node::decorator::Decorator>, std::string> parseDecorator(const pugi::xml_node &node, const int &index, const DecoratorType &decorator_type)
        {
            const std::string name_attribute = node.attribute("name").as_string();
            const pugi::xml_node child = node.first_child();
            if (!child)
            {
                return tl::unexpected(fmt::format(R"(Decorator node must have a child | {}:['{}',{}])", std::string(node.name()), name_attribute, index));
            }
            auto maybe_child_node = parseChild(child, STARTING_INDEX);
            if (!maybe_child_node.has_value())
            {
                return tl::unexpected(maybe_child_node.error());
            }
            switch (decorator_type)
            {
            case DecoratorType::Invert:
            {
                return std::make_shared<Invert>(
                    Invert(
                        name_attribute,
                        std::move(maybe_child_node.value())));
            }
            case DecoratorType::Repeat:
            {
                std::string amount_string = node.attribute("amount").as_string();
                // Convert to lower case: https://stackoverflow.com/a/313990
                std::transform(amount_string.begin(), amount_string.end(), amount_string.begin(),
                               [](unsigned char c)
                               { return std::tolower(c); });
                unsigned long count = 0;
                if (amount_string == "inf")
                {
                    count = std::numeric_limits<unsigned long>::max();
                }
                else
                {
                    try
                    {
                        count = std::stoll(amount_string);
                        if (count < 0)
                        {
                            return tl::unexpected(fmt::format(R"(Invalid amount: '{}' | Repeat:['{}',{}])", amount_string, name_attribute, index));
                        }
                    }
                    catch (const std::exception &_)
                    {
                        return tl::unexpected(fmt::format(R"(Invalid amount: '{}' | Repeat:['{}',{}])", amount_string, name_attribute, index));
                    }
                }

                return std::make_shared<Repeat>(
                    Repeat(
                        name_attribute,
                        std::move(maybe_child_node.value()),
                        count,
                        node.attribute("break_on_fail").as_bool(false)));
            }
            }
        }

        tl::expected<std::shared_ptr<Composite>, std::string> parseComposite(const pugi::xml_node &node, const int &index, const CompositeType &composite_type)
        {
            const std::string name_attribute = node.attribute("name").as_string();
            std::vector<std::shared_ptr<node::Node>> children;
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
            {
                auto maybe_node = parseChild(child, STARTING_INDEX + children.size());
                if (!maybe_node.has_value())
                {
                    return tl::unexpected(fmt::format(R"({}<-{}:['{}',{}])", maybe_node.error(), std::string(node.name()), name_attribute, index));
                }
                children.push_back(std::move(maybe_node.value()));
            }
            if (children.size() <= 0)
            {
                return tl::unexpected(fmt::format(R"(Composite node must have at least one child | {}:['{}',{}])", std::string(node.name()), name_attribute, index));
            }
            switch (composite_type)
            {
            case CompositeType::Sequence:
                return std::make_shared<Sequence>(Sequence(name_attribute, std::move(children)));
            case CompositeType::Selector:
                return std::make_shared<Selector>(Selector(name_attribute, std::move(children)));
            case CompositeType::Random:
                return std::make_shared<Random>(Random(name_attribute, std::move(children)));
            }
        }

        tl::expected<std::shared_ptr<node::custom::CustomNode>, std::string> parseCustomNode(const pugi::xml_node &node, const int &index)
        {
            return this->custom_node_parser->parseCustomNode(node, index);
        }
    };
}

#endif