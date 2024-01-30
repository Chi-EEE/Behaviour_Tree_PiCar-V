#ifndef LOGGINGSCREEN_CXX
#define LOGGINGSCREEN_CXX

#pragma once

#include <ftxui/component/component.hpp>
#include <spdlog/spdlog.h>

#include "../../system/logging/VectorSink.cxx"

using namespace ftxui;
using namespace car::system;

namespace car::display::screen
{
	class LoggingScreen
	{
	public:
		LoggingScreen(std::shared_ptr<logging::vector_sink_mt> vector_sink) : vector_sink(vector_sink){};

		Component element()
		{
			// https://github.com/ArthurSonzogni/FTXUI/discussions/304
			menu = Menu(&this->vector_sink->get_log_messages(), &selected_line);

			my_custom_menu = Renderer(menu, [&]
									  {
				int max_lines_shown = Terminal::Size().dimy - 5;
				int begin = selected_line - max_lines_shown / 2;
				int end = selected_line + max_lines_shown / 2;
				if (begin < 0) {
					begin = 0;
					end = std::min(max_lines_shown, static_cast<int>(this->vector_sink->get_log_messages().size()));
				}
				else if (end > static_cast<int>(this->vector_sink->get_log_messages().size())) {
					end = static_cast<int>(this->vector_sink->get_log_messages().size());
					begin = std::max(0, end - max_lines_shown);
				}
				Elements elements;
				for (int i = begin; i < end; ++i) {
					Element element = text(this->vector_sink->get_log_messages()[i]);
					if (i == selected_line)
						element = element | inverted;
					elements.push_back(element);
				}
				return vbox(std::move(elements)) | vscroll_indicator | frame | border; });
			return my_custom_menu;
		}

	private:
		int selected_line = 0;

		std::shared_ptr<logging::vector_sink_mt> vector_sink;

		Component menu;
		Component my_custom_menu;

		ftxui::Elements line_elements;
	};
}

#endif