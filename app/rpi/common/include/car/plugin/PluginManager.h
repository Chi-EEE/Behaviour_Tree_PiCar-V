#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#pragma once

#include <vector>
#include <memory>

#include "utils/Utility.hpp"
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
			for (std::shared_ptr<Plugin>& plugin : this->plugins)
			{
				plugin->initialize(car_system);
			}
		}

		void update()
		{
			for (std::shared_ptr<Plugin>& plugin : this->plugins)
			{
				plugin->update();
			}
		}

		void stop()
		{
			for (std::shared_ptr<Plugin>& plugin : this->plugins)
			{
				plugin->stop();
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
            std::string type_name = std::string(utils::TypeName<T>());
			type_name = utils::getStringAfterLastColon(type_name);

			for (std::shared_ptr<Plugin>& plugin : this->plugins)
			{
				if (plugin->getName() == type_name)
				{
					return plugin;
				}
			}

			return nullptr;
		}


	private:
		std::vector<std::shared_ptr<Plugin>> plugins;
	};
}

#endif