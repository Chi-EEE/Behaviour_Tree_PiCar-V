#ifndef UTILITY_HPP
#define UTILITY_HPP

#pragma once

#include <string>
#include <sstream>
#include <vector>

class Utility
{
public:
	// https://stackoverflow.com/a/46931770
	static std::vector<std::string> split(const std::string& s, const int& from_index, const int& to_index, char delim) {
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;
		int index = 0;

		while (getline(ss, item, delim)) {
			if (index >= from_index && index < to_index)
				result.push_back(item);
			index++;
		}

		return result;
	}
};

#endif