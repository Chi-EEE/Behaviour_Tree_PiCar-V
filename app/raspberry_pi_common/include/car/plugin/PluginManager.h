#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#pragma once

#include <vector>
#include <memory>

#include "utils/TypeName.hpp"

#include "Plugin.h"

namespace car::system
{
	class CarSystem;
}

namespace car::plugin
{
	class PluginManager
	{
	public:
		void initialize(std::shared_ptr<system::CarSystem> car_system)
		{
			for (std::weak_ptr<Plugin>& plugin : this->plugins)
			{
				plugin.lock()->initialize(car_system);
			}
		}

		void update()
		{
			for (std::weak_ptr<Plugin>& plugin : this->plugins)
			{
				plugin.lock()->update();
			}
		}

		void stop()
		{
			for (std::weak_ptr<Plugin>& plugin : this->plugins)
			{
				plugin.lock()->stop();
			}
		}

		void terminate()
		{
			this->stop();
		}

		void addPlugin(std::shared_ptr<Plugin> plugin)
		{
			this->plugins.push_back(plugin);
		}

		template<typename T>
		std::shared_ptr<T> getPlugin()
		{
			static_assert(std::is_base_of<Plugin, T>::value, "T must be a Plugin");
			auto type_name = utils::getStringAfterLastColon(utils::TypeName<T>());

			for (std::weak_ptr<Plugin>& plugin : this->plugins)
			{
				if (auto locked_plugin = plugin.lock())
				{
					if (locked_plugin->getName() == type_name)
					{
						return std::static_pointer_cast<T>(locked_plugin);
					}
				}
			}

			return nullptr;
		}


	private:
		std::vector<std::weak_ptr<Plugin>> plugins;
	};
}

#endif