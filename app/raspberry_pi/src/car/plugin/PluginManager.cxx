#ifndef PLUGIN_MANAGER_CXX
#define PLUGIN_MANAGER_CXX

#pragma once

#include <vector>
#include <memory>

#include "Plugin.cxx"

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
			for (std::weak_ptr<Plugin> &plugin : this->plugins)
			{
				plugin.lock()->initialize(car_system);
			}
		}

		void update()
		{
			for (std::weak_ptr<Plugin> &plugin : this->plugins)
			{
				plugin.lock()->update();
			}
		}

		void stop()
		{
			for (auto &plugin : this->plugins)
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

	private:
		std::vector<std::weak_ptr<Plugin>> plugins;
	};
}

#endif