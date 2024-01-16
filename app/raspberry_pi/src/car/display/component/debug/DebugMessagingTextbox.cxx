#ifndef DEBUGMESSAGINGTEXTBOX_CXX
#define DEBUGMESSAGINGTEXTBOX_CXX

#pragma once

#include <nod/nod.hpp>

#include <ftxui/component/component.hpp>

using namespace ftxui;

namespace car::display::component::debug {
	class DebugMessagingTextbox {
	public:
		DebugMessagingTextbox(nod::signal<void(const std::string)>& handle_message_signal) : handle_message_signal(handle_message_signal)
		{

		}

		ftxui::Component element()
		{
			this->messaging_textbox = Input(&this->message, "Enter a JSON message");
			this->messaging_button = Button("Click here to send a message to the MessagingSystem", [&]
				{
					this->handle_message_signal(this->message);
					this->message = "";
				}
			);

			this->messaging_container =
				Container::Vertical({
					this->messaging_textbox
					});

			return Renderer(this->messaging_container, [&] {
				return
					vbox({
						this->messaging_textbox->Render(),
						this->messaging_button->Render(),
						});
				});
		}

	private:
		std::string message;

		Component messaging_textbox;
		Component messaging_button;

		Component messaging_container;

		nod::signal<void(const std::string)>& handle_message_signal;
	};
}

#endif