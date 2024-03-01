#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#pragma once

#include <optional>
#include <string>

#include <tl/expected.hpp>

namespace car::configuration
{
	struct Configuration
	{
		std::string ip_address;
		std::optional<int> port = std::nullopt;
		std::string name;
		std::string room;
	};
};

#endif