#ifndef MOVECOMMAND_HPP
#define MOVECOMMAND_HPP

#pragma once

namespace car_system::messaging::commands {
	struct MoveCommand
	{
		int speed;
	};
};

#endif