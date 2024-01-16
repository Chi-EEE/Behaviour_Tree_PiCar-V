#ifndef VECTORSINK_CXX
#define VECTORSINK_CXX

#include <algorithm>
#include <vector>

#include <fmt/format.h>

#include <ftxui/dom/elements.hpp>

#include <spdlog/sinks/base_sink.h>

namespace car::display::logging {
	template<typename Mutex>
	class VectorSink : public spdlog::sinks::base_sink<Mutex> {
	public:
		VectorSink(int max_lines) : max_lines(max_lines)
		{
			this->log_messages.reserve(this->max_lines);
			for (size_t i = 0; i < max_lines; i++)
			{
				this->log_messages.push_back("");
			}
		}

		void sink_it_(const spdlog::details::log_msg& msg) override {
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
			if (log_messages.size() < max_lines)
			{
				this->log_messages.push_back(std::string(formatted.data(), formatted.size()));
			}
			else
			{
				std::rotate(this->log_messages.begin(), this->log_messages.begin() + 1, this->log_messages.end());
				this->log_messages[this->log_messages.size() - 1] = std::string(formatted.data(), formatted.size());
			}
		};

		void flush_() override {};

		inline auto element()
		{
			ftxui::Elements line_elements;
			for (auto& el : this->log_messages)
			{
				line_elements.push_back(ftxui::text_element(el.msg, el.msg_color));
			}

			if (enable_flex)
				return hbox(
					window(text(name), vbox(std::move(line_elements)) | flex));
			else
				return hbox(
					window(text(name), vbox(std::move(line_elements))));
		}

	private:
		const int max_lines;

		std::vector<std::string> log_messages;
	};
}

#endif