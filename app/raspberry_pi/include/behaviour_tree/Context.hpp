#ifndef BEHAVIOUR_TREE_CONTEXT_HPP
#define BEHAVIOUR_TREE_CONTEXT_HPP

#pragma once

namespace behaviour_tree
{
	class Context
	{
	public:
		Context()
		{
		}

		void log(std::string message)
		{
			this->messages.push_back(message);
		}

	private:
		std::vector<std::string> messages;
	};
}

#endif