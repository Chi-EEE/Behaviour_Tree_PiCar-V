#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#pragma once

#include <optional>
#include <string>

#include <tl/expected.hpp>

namespace car::configuration
{
	class Configuration
	{
	public:
		Configuration(std::string exe_dir);
		Configuration(Configuration const&) = delete;
		Configuration& operator=(Configuration const&) = delete;

		void setConfigFilePath(std::string config_file_path);
		const std::string& getConfigFilePath() const;
		tl::expected<nullptr_t, std::string> load();

	public:
		std::string ip_address;
		std::optional<int> port = std::nullopt;

		std::string name;
		std::string room;

	private:
		const std::string exe_dir;
		std::string config_file_path;
	};
};

#endif