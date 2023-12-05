#ifndef TURNCOMMAND_HPP
#define TURNCOMMAND_HPP

#pragma once

namespace car_system::messaging::commands {
	struct TurnCommand
	{
		float angle;
	};
};

#endif