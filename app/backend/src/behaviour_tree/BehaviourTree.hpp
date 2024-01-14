#ifndef BEHAVIOUR_TREE_HPP
#define BEHAVIOUR_TREE_HPP

#pragma once

/*
XML format inspired by: https://github.com/telcy/modular-behavior-tree & https://github.com/Defaultio/BehaviorTree3
*/

#include <filesystem>
#include <string>

#include <tl/expected.hpp>
#include <pugixml.hpp>

#include "Root.hpp"
#include "node/Node.hpp"

#include "node/composite/Composite.hpp"
#include "node/composite/Sequence.hpp"
#include "node/composite/Selector.hpp"

#include "node/decorator/Decorator.hpp"
#include "node/decorator/Repeat.hpp"
#include "node/decorator/Invert.hpp"
#include "node/decorator/Wait.hpp"

#include "node/leaf/Leaf.hpp"
#include "node/leaf/Task.hpp"
#include "node/leaf/Succeed.hpp"
#include "node/leaf/Fail.hpp"
#include "node/leaf/LogMessage.hpp"
#include "node/leaf/ToRoot.hpp"

#include "node/leaf/Condition.hpp"
#include "node/leaf/condition/NearbyPoints.hpp"

#include "action/Action.hpp"
#include "action/Direction.hpp"
#include "action/Turn.hpp"
#include "action/Move.hpp"

using namespace behaviour_tree::node;
using namespace behaviour_tree::node::composite;
using namespace behaviour_tree::node::decorator;
using namespace behaviour_tree::node::leaf;
using namespace behaviour_tree::node::leaf::condition;
using namespace behaviour_tree::action;

namespace behaviour_tree
{
	static constexpr uint32_t hash(const std::string_view s) noexcept
	{
		uint32_t hash = 5381;

		for (const char* c = s.data(); c < s.data() + s.size(); ++c)
			hash = ((hash << 5) + hash) + (unsigned char)*c;

		return hash;
	}

	class BehaviourTree
	{
	public:
		BehaviourTree(std::vector<std::unique_ptr<Root>> roots) : roots(std::move(roots))
		{
		}

		std::vector<std::unique_ptr<Root>> roots;

		void run()
		{
			for (auto& root : this->roots)
			{
				if (root->getId() == "Main")
				{
					root->run();
					break;
				}
			}
		}

		static tl::expected<std::unique_ptr<BehaviourTree>, std::string> parseFileXML(const std::string& file_path)
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
			int child_count = 0;
			for (pugi::xml_node child = doc.first_child(); child; child = doc.next_sibling())
			{
				const std::string& child_name = child.name();
				if (child_name == "") // Skip empty nodes
					continue;
				if (child_name != "Root")
				{
					return tl::unexpected("Invalid node: " + child_name + "");
				}
				++child_count;
			}
			if (child_count <= 0)
			{
				return tl::unexpected("No root node found");
			}
			std::vector<std::unique_ptr<Root>> roots;
			for (pugi::xml_node node = doc.child("Root"); node; node = node.next_sibling("Root"))
			{
				auto maybe_root = parseRoot(node);
				if (!maybe_root.has_value())
				{
					return tl::unexpected(maybe_root.error());
				}
				roots.push_back(std::move(maybe_root.value()));
			}
			return std::make_unique<BehaviourTree>(
				BehaviourTree(std::move(roots))
			);
		}

		static tl::expected<std::unique_ptr<Root>, std::string> parseRoot(pugi::xml_node& node)
		{
			int child_count = 0;
			for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
				++child_count;
			if (child_count > 1)
			{
				return tl::unexpected("Root node " + std::string(node.attribute("id").as_string()) + " must have only one child");
			}
			auto child = node.first_child();
			auto maybe_child_node = parseChild(child);
			if (!maybe_child_node.has_value())
			{
				return tl::unexpected(maybe_child_node.error());
			}
			return std::make_unique<Root>(
				Root(node.attribute("id").as_string(), std::move(maybe_child_node.value()))
			);
		}

		static tl::expected<std::unique_ptr<Node>, std::string> parseChild(pugi::xml_node& node)
		{
			const std::string& node_name = node.name();
			const auto node_hash = hash(node_name);
			switch (node_hash)
			{
#pragma region Composite Node
			case hash("Sequence"):
			{
				return parseComposite(node, CompositeType::Sequence);
			}
			case hash("Selector"):
			{
				return parseComposite(node, CompositeType::Selector);
			}
#pragma endregion
#pragma region Decorator Node
			case hash("Repeat"):
			{
				return parseDecorator(node, DecoratorType::Repeat);
			}
			case hash("Invert"):
			{
				return parseDecorator(node, DecoratorType::Invert);
			}
			case hash("Wait"):
			{
				return parseDecorator(node, DecoratorType::Wait);
			}
#pragma endregion
#pragma region Leaf Node
			case hash("Condition"):
			{
				return parseCondition(node);
			}
			case hash("Task"):
			{
				return parseTask(node);
			}
			case hash("Succeed"):
			{
				return std::make_unique<Succeed>(Succeed(node.attribute("name").as_string()));
			}
			case hash("Fail"):
			{
				return std::make_unique<Fail>(Fail(node.attribute("name").as_string()));
			}
			case hash("LogMessage"):
			{
				return std::make_unique<LogMessage>(
					LogMessage(
						node.attribute("name").as_string(),
						node.attribute("text").as_string()));
			}
			case hash("ToRoot"):
			{
				return std::make_unique<ToRoot>(
					ToRoot(
						node.attribute("name").as_string(),
						node.attribute("id").as_string()));
			}
#pragma endregion
			default:
			{
				return tl::unexpected("Invalid node: " + node_name);
			}
			}
		}

		static tl::expected<std::unique_ptr<Decorator>, std::string> parseDecorator(pugi::xml_node& node, const DecoratorType& decorator_type)
		{
			auto child = node.first_child();
			if (!child)
			{
				return tl::unexpected("Decorator node " + std::string(node.name()) + " must have a child");
			}
			auto maybe_child_node = parseChild(child);
			if (!maybe_child_node.has_value())
			{
				return tl::unexpected(maybe_child_node.error());
			}
			switch (decorator_type)
			{
			case DecoratorType::Invert:
			{
				return std::make_unique<Invert>(
					Invert(
						node.attribute("name").as_string(),
						std::move(maybe_child_node.value())));
			}
			case DecoratorType::Repeat:
			{
				return std::make_unique<Repeat>(
					Repeat(
						node.attribute("name").as_string(),
						std::move(maybe_child_node.value()),
						node.attribute("count").as_int(),
						node.attribute("break_on_fail").as_bool()));
			}
			case DecoratorType::Wait:
			{
				return std::make_unique<Wait>(
					Wait(
						node.attribute("name").as_string(),
						std::move(maybe_child_node.value()),
						node.attribute("ms").as_int()));
			}
			}
		}

		static tl::expected<std::unique_ptr<Composite>, std::string> parseComposite(pugi::xml_node& node, const CompositeType& composite_type)
		{
			std::vector<std::unique_ptr<Node>> children;
			for (auto child = node.first_child(); child; child = child.next_sibling())
			{
				auto maybe_node = parseChild(child);
				if (!maybe_node.has_value())
				{
					return tl::unexpected(maybe_node.error());
				}
				children.push_back(std::move(maybe_node.value()));
			}
			if (children.size() <= 0)
			{
				return tl::unexpected("Composite node " + std::string(node.name()) + " must have at least one child");
			}
			switch (composite_type)
			{
			case CompositeType::Sequence:
				return std::make_unique<Sequence>(Sequence(node.attribute("name").as_string(), std::move(children)));
			case CompositeType::Selector:
				return std::make_unique<Selector>(Selector(node.attribute("name").as_string(), std::move(children)));
			}
		}

		static tl::expected<std::unique_ptr<Condition>, std::string> parseCondition(pugi::xml_node& node)
		{
			switch (hash(node.attribute("type").as_string()))
			{
			case hash("NearbyPoints"):
				return std::make_unique<NearbyPoints>(
					NearbyPoints(
						node.attribute("name").as_string(),
						node.attribute("min_angle").as_int(),
						node.attribute("max_angle").as_int(),
						node.attribute("avg_distance").as_int()
					)
				);
			}
		}

		static tl::expected<std::unique_ptr<Task>, std::string> parseTask(pugi::xml_node& node)
		{
			std::vector<std::unique_ptr<Action>> actions;
			for (pugi::xml_node child = node.child("Action"); child; child = child.next_sibling("Action"))
			{
				auto maybe_action = parseAction(child);
				if (!maybe_action.has_value())
				{
					return tl::unexpected(maybe_action.error());
				}
				actions.push_back(std::move(maybe_action.value()));
			}
			return std::make_unique<Task>(Task(node.attribute("name").as_string(), std::move(actions)));
		}

		static tl::expected<std::unique_ptr<Action>, std::string> parseAction(pugi::xml_node& node)
		{
			const std::string name = node.attribute("name").as_string();
			const std::string type = node.attribute("type").as_string();
			switch (hash(type))
			{
			case hash("Move"):
			{
				const int ms = node.attribute("ms").as_int();
				if (ms < 0)
					return tl::unexpected("Invalid ms: " + std::to_string(ms));
				return std::make_unique<Move>(Move(ms));
			}
			case hash("Turn"):
			{
				const int angle = node.attribute("angle").as_int();
				if (angle < 0 || angle > 180)
					return tl::unexpected("Invalid angle: " + std::to_string(angle));
				return std::make_unique<Turn>(Turn(angle));
			}
			case hash("Direction"):
			{
				const std::string direction_type_attribute = node.attribute("direction_type").as_string();
				if (direction_type_attribute == "Forward")
					return std::make_unique<Direction>(Direction(DirectionType::Forward));
				else if (direction_type_attribute == "Backward")
					return std::make_unique<Direction>(Direction(DirectionType::Backward));
				else
					return tl::unexpected("Invalid direction_type: " + direction_type_attribute);
			}
			default:
			{
				return tl::unexpected("Invalid action type: " + type);
			}
			}
		}
	};
}

#endif