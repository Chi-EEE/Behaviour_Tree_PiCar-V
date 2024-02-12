#ifndef UTILITY_HPP
#define UTILITY_HPP

#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace utils {
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

	// https://stackoverflow.com/a/46711735
	static constexpr uint32_t hash(const std::string_view s) noexcept
	{
		uint32_t hash = 5381;

		for (const char* c = s.data(); c < s.data() + s.size(); ++c)
			hash = ((hash << 5) + hash) + (unsigned char)*c;

		return hash;
	}

	// From: https://stackoverflow.com/a/5665377
	// To escape HTML characters
	static void encode(std::string& data) {
		std::string buffer;
		buffer.reserve(data.size());
		for (size_t pos = 0; pos != data.size(); ++pos) {
			switch (data[pos]) {
			case '&':  buffer.append("&amp;");       break;
			case '\"': buffer.append("&quot;");      break;
			case '\'': buffer.append("&apos;");      break;
			case '<':  buffer.append("&lt;");        break;
			case '>':  buffer.append("&gt;");        break;
			default:   buffer.append(&data[pos], 1); break;
			}
		}
		data.swap(buffer);
	}

	// To get class name without namespace
	static std::string getStringAfterLastColon(const std::string& input) {
		size_t last_colon_pos = input.find_last_of("::");

		if (last_colon_pos != std::string::npos) {
			return input.substr(last_colon_pos + 1);
		}
		else {
			return input;
		}
	}
}

#endif