#ifndef TURNCOMMAND_HPP
#define TURNCOMMAND_HPP

#pragma once

namespace car::system::messaging::commands {
	struct TurnCommand
	{
		float angle;
	};
};

#endif