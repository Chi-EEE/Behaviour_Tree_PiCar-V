#ifndef DEBUGMESSAGINGTEXTBOX_CXX
#define DEBUGMESSAGINGTEXTBOX_CXX

#pragma once

#include <nod/nod.hpp>

#include <ftxui/component/component.hpp>

using namespace ftxui;

namespace car::display::console::component::debug
{
	class DebugMessagingTextbox
	{
	public:
		DebugMessagingTextbox(nod::signal<void(const std::string)> &handle_message_signal) : handle_message_signal(handle_message_signal)
		{
			this->messaging_title = MenuEntry("Simulate the MessagingSystem:") | bold;

			InputOption messaging_textbox_option = InputOption();
			messaging_textbox_option.on_enter = [&]()
			{
				this->handle_message_signal(this->message);
				this->message = "";
			};
			this->messaging_textbox = Input(&this->message, "[Enter text here and press enter to send the message!]", messaging_textbox_option) | dim;

			this->messaging_container =
				Container::Vertical({
					this->messaging_title,
					this->messaging_textbox,
				});
		}

		ftxui::Component element()
		{
			return Renderer(this->messaging_container, [&]
							{ return vbox(
								  {
									  this->messaging_title->Render(),
									  separator(),
									  this->messaging_textbox->Render(),
								  }); });
		}

	private:
		std::string message;

		Component messaging_title;
		Component messaging_textbox;

		Component messaging_container;

		nod::signal<void(const std::string)> &handle_message_signal;
	};
}

#endif