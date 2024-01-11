#ifndef DEBUGENABLER_CXX
#define DEBUGENABLER_CXX

#pragma once

#include <nod/nod.hpp>

#include <ftxui/component/component.hpp>

using namespace ftxui;

namespace car::display::component::debug {
	class DebugEnabler {
	private:
		static constexpr auto DEBUG_ENABLE_WARNING_MESSAGE = "Enabling debug mode temporarily disables connecting to online. Are you sure you want to do this?";

		static constexpr auto DEBUG_MODE_ENABLED_MESSAGE = "Debug Status: Enabled";
		static constexpr auto DEBUG_MODE_DISABLED_MESSAGE = "Debug Status: Disabled";
		static constexpr auto DEBUG_MODE_WAIT_MESSAGE = "Debug Status: Waiting for user input...";
	public:
		Component getCheckbox() {
			auto debug_checkbox_option = CheckboxOption::Simple();
			debug_checkbox_option.on_change = [&]
				{
					if (this->debounce) {
						this->checkbox_value = !this->checkbox_value;
						return;
					}
					this->debounce = true;
					if (this->enabled) {
						this->status = DEBUG_MODE_DISABLED_MESSAGE;
						this->enabled = false;
						this->debounce = false;
					}
					else {
						this->display_warn_debug_modal = true;
						this->status = DEBUG_MODE_WAIT_MESSAGE;
					}
				};

			return Checkbox(&this->status, &this->checkbox_value, debug_checkbox_option);
		}

		ComponentDecorator getWarningModal() {
			auto enable_debug_mode = [&] {
				this->enabled = true;
				this->display_warn_debug_modal = false;
				this->status = DEBUG_MODE_ENABLED_MESSAGE;
				this->debounce = false;
				};

			auto hide_enable_debug_warning_modal = [&] {
				this->checkbox_value = false;
				this->display_warn_debug_modal = false;
				this->status = DEBUG_MODE_DISABLED_MESSAGE;
				this->debounce = false;
				};

			this->component = Container::Vertical({
				Button("No", hide_enable_debug_warning_modal, ButtonOption::Animated()),
				Button("Yes", enable_debug_mode, ButtonOption::Animated()),
				}
			);

			this->component |= Renderer([&](Element inner) {
				return vbox({
						   text(DEBUG_ENABLE_WARNING_MESSAGE),
						   separator(),
						   inner,
					})
					| size(WIDTH, GREATER_THAN, 30)
					| border;
				});

			return Modal(this->component, &display_warn_debug_modal);
		}
		
		const bool& isEnabled() {
			return this->enabled;
		}
	private:
		bool debounce = false;
		bool enabled = false;
		bool checkbox_value = false;
		bool display_warn_debug_modal = false;
		std::string status = DEBUG_MODE_DISABLED_MESSAGE;

		Component component;
    };
}

#endif