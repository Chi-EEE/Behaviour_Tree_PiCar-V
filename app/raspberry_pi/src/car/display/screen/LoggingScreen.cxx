#ifndef LOGGINGSCREEN_CXX
#define LOGGINGSCREEN_CXX

#pragma once

#include <ftxui/component/component.hpp>
#include "../../system/logging/VectorSink.cxx"

using namespace ftxui;
using namespace car::system;

namespace car::display::screen {
	class LoggingScreen {
	public:
		LoggingScreen(std::shared_ptr<logging::vector_sink_mt> vector_sink) : vector_sink(vector_sink) {
		};

		Component element() {
			return Renderer(
				[&] {
					// TODO: Improve this so it doesn't create a log of lines
					this->line_elements.clear();
					for (const std::string& message : this->vector_sink->get_log_messages())
					{
						this->line_elements.push_back(paragraph(message));
						this->line_elements.push_back(separator());
					}
					// TODO: Make the log scrollable
					return vbox(this->line_elements) | xflex | vscroll_indicator | frame;
				}
			);
		}

	private:
		std::shared_ptr<logging::vector_sink_mt> vector_sink;

		ftxui::Elements line_elements;
	};
}

#endif