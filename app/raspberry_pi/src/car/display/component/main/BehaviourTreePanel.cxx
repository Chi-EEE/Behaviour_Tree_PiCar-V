#ifndef BEHAVIOURTREEPANEL_CXX
#define BEHAVIOURTREEPANEL_CXX

#pragma once

#include <ftxui/component/component.hpp>

#include "behaviour_tree/BehaviourTreeHandler.hpp"

#include "../../../system/CarSystem.h"

using namespace ftxui;
using namespace car::system;

namespace car::display::component::main
{
	struct BehaviourTreeItem {
		Component delete_button = Button("X", [&] {}, ButtonOption::Animated());
		Component info_button = Button("Info", [&] {}, ButtonOption::Animated());
		Component state_button = Button("Start", [&] {}, ButtonOption::Animated());
	};

	class BehaviourTreePanel
	{
	public:
		BehaviourTreePanel(std::shared_ptr<CarSystem> car_system) : car_system(car_system)
		{
			this->elelm = Renderer(Container::Vertical(this->behaviour_tree_list_components, 0), [&] {
				auto behaviour_tree_handler = this->car_system->getPlugin<behaviour_tree::BehaviourTreeHandler>();
				this->behaviour_tree_list.clear();
				this->behaviour_tree_list_components.clear();
				for (auto& behaviour_tree : behaviour_tree_handler->getBehaviourTrees())
				{
					BehaviourTreeItem item;
					item.delete_button = Button("X", [&] {}, ButtonOption::Animated());
					item.info_button = Button("Info", [&] {}, ButtonOption::Animated());
					item.state_button = Button("Start", [&] {}, ButtonOption::Animated());
					this->behaviour_tree_list.push_back(item);

					this->behaviour_tree_list_components.push_back(item.delete_button);
					this->behaviour_tree_list_components.push_back(item.info_button);
					this->behaviour_tree_list_components.push_back(item.state_button);

					this->behaviour_tree_list_elements.push_back(
						hbox({
							 item.delete_button->Render(),
							 text("Behaviour Tree: ") | vcenter,
							 separator(),
							 vbox({
							 item.info_button->Render(),
							 item.state_button->Render(),
						   })}) | border);
				}
				behaviour_tree_handler->getBehaviourTrees();
				return vbox({
					  text("Behaviour Tree Queue"),
					  separator(),
					  vbox(std::move(this->behaviour_tree_list_elements)),
					});
				}
			);
		}
		Component element()
		{
			return elelm;
		}

	private:
		std::shared_ptr<CarSystem> car_system;

		std::vector<BehaviourTreeItem> behaviour_tree_list;
		Components behaviour_tree_list_components;
		Elements behaviour_tree_list_elements;

		Component behaviour_tree_list_component;


		Component elelm;
	};
}

#endif