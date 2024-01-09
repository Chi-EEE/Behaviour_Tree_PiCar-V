#include "CarConsole.h"

namespace car::display {
	Component CarConsole::MainComponent(Component main_button, std::function<void()> show_exit_modal) {
		auto component = Container::Vertical({
			main_button,
			Button("Quit", show_exit_modal, animated_button_style),
			});
		// Polish how the two buttons are rendered:
		component |= Renderer([&](Element inner) {
			return vbox({
					   text("Main component"),
					   separator(),
					   inner,
				})                                //
				| size(WIDTH, GREATER_THAN, 15)   //
				| size(HEIGHT, GREATER_THAN, 15)  //
				| border                          //
				| center;                         //
			});
		return component;
	}

	Component CarConsole::ExitModalComponent(
		std::function<void()> hide_exit_modal,
		std::function<void()> exit
	) {
		auto component = Container::Vertical({
			Button("No", hide_exit_modal, animated_button_style),
			Button("Yes", exit, animated_button_style),
			});
		component |= Renderer([&](Element inner) {
			return vbox({
					   text("Are you sure you want to exit?"),
					   separator(),
					   inner,
				})
				| size(WIDTH, GREATER_THAN, 30)
				| border;
			});
		return component;
	}

	Component CarConsole::DebugEnableWarningComponent(
		std::function<void()> hide_enable_debug_warning_modal,
		std::function<void()> enable_debug_mode
	) {
		static constexpr auto DEBUG_ENABLE_WARNING_MESSAGE = "Enabling debug mode temporarily disables connecting to online. Are you sure you want to do this?";

		auto component = Container::Vertical({
			Button("No", hide_enable_debug_warning_modal, animated_button_style),
			Button("Yes", enable_debug_mode, animated_button_style),
			});
		component |= Renderer([&](Element inner) {
			return vbox({
					   text(DEBUG_ENABLE_WARNING_MESSAGE),
					   separator(),
					   inner,
				})
				| size(WIDTH, GREATER_THAN, 30)
				| border;
			});
		return component;
	}

	CarConsole::CarConsole(std::unique_ptr<CarSystem> car_system) : car_system(std::move(car_system)) {
	};

	void CarConsole::initialize() {
		this->car_system->initialize();
	};

	void CarConsole::run() {
		auto screen = ScreenInteractive::Fullscreen();
#pragma region Main Screen
#pragma region Main Button
		bool main_debounce = false;
		bool button_pressed = false;
		std::string main_button_text = "Start Car Application";

		auto main_button_lambda = [&] {
			if (main_debounce) return;
			main_debounce = true;
			button_pressed = !button_pressed;
			std::cout << "Button pressed: " << button_pressed << std::endl;
			if (button_pressed) {
				main_button_text = "Connecting...";
				this->car_system->start();
				main_button_text = "Stop Car Application";
			}
			else {
				main_button_text = "Disconnecting...";
				this->car_system->stop();
				main_button_text = "Start Car Application";
			}
			main_debounce = false;
			};

		auto main_button = Button(&main_button_text, main_button_lambda, animated_button_style);
#pragma endregion

#pragma region Exit Modal
		bool exit_modal_shown = false;

		auto show_exit_modal = [&] { exit_modal_shown = true; };
		auto hide_exit_modal = [&] { exit_modal_shown = false; };

		auto exit = screen.ExitLoopClosure();

		auto modal_component = ExitModalComponent(hide_exit_modal, exit);
#pragma endregion

		auto main_component = MainComponent(main_button, show_exit_modal);
		main_component |= Modal(modal_component, &exit_modal_shown);
#pragma endregion

#pragma region Lidar Scan Map
		auto renderer_line_block = Renderer([&] {
			auto c = Canvas(100, 100);
			for (auto& point : this->car_system->getScanData()) {
				const double angle = point.angle;
				const double distance = point.distance;
				const double angleInRadians = angle * (3.14159265f / 180.0f);
				const int x = distance * std::cos(angleInRadians);
				const int y = distance * std::sin(angleInRadians);
				c.DrawBlockLine(50, 50, x, y, Color::Blue);
			}
			return canvas(std::move(c));
			}
		);
#pragma endregion

#pragma region Settings Screen

#pragma region Debug Modal		
		bool debug_enabled = false;

		static constexpr auto DEBUG_MODE_ENABLED_MESSAGE = "Debug Status: Enabled";
		static constexpr auto DEBUG_MODE_DISABLED_MESSAGE = "Debug Status: Disabled";
		static constexpr auto DEBUG_MODE_WAIT_MESSAGE = "Debug Status: Waiting for user input...";

		std::string debug_status = DEBUG_MODE_DISABLED_MESSAGE;

		bool debug_debounce = false;
		bool debug_checkbox_value = false;
		bool display_warn_debug_modal = false;
		auto debug_checkbox_option = CheckboxOption::Simple();

		debug_checkbox_option.on_change = [&]
			{
				if (debug_debounce) {
					debug_checkbox_value = !debug_checkbox_value;
					return;
				}
				debug_debounce = true;
				if (debug_enabled) {
					debug_status = DEBUG_MODE_DISABLED_MESSAGE;
					debug_enabled = false;
					debug_debounce = false;
				}
				else {
					display_warn_debug_modal = true;
					debug_status = DEBUG_MODE_WAIT_MESSAGE;
				}
			};

		auto debug_checkbox_component = Checkbox(&debug_status, &debug_checkbox_value, debug_checkbox_option);

		auto enable_debug_mode = [&] {
			debug_enabled = true;
			display_warn_debug_modal = false;
			debug_status = DEBUG_MODE_ENABLED_MESSAGE;
			debug_debounce = false;
			};

		auto hide_enable_debug_warning_modal = [&] {
			debug_checkbox_value = false;
			display_warn_debug_modal = false;
			debug_status = DEBUG_MODE_DISABLED_MESSAGE;
			debug_debounce = false;
			};

		auto warn_enable_debug_modal = Modal(DebugEnableWarningComponent(hide_enable_debug_warning_modal, enable_debug_mode), &display_warn_debug_modal);
#pragma endregion

#pragma region Lidar Scanner Checkbox
		static constexpr auto LIDAR_ENABLED_MESSAGE = "Lidar Status: Enabled";
		static constexpr auto LIDAR_DISABLED_MESSAGE = "Lidar Status: Disconnected";
		static constexpr auto LIDAR_WAIT_ENABLED_MESSAGE = "Lidar Status: Enabling...";
		static constexpr auto LIDAR_WAIT_DISABLED_MESSAGE = "Lidar Status: Disabling...";

		nod::signal<void(bool)> lidar_signal;

		bool lidar_loading_debounce = false;
		std::string lidar_status = LIDAR_DISABLED_MESSAGE;
		bool lidar_enabled = false;
		auto lidar_checkbox_option = CheckboxOption::Simple();
		lidar_checkbox_option.on_change = [&]
			{
				if (lidar_loading_debounce) {
					lidar_enabled = !lidar_enabled;
					return;
				}
				lidar_loading_debounce = true;
				if (lidar_enabled) {
					lidar_status = LIDAR_WAIT_ENABLED_MESSAGE;
				}
				else {
					lidar_status = LIDAR_WAIT_DISABLED_MESSAGE;
				}
				lidar_signal(lidar_enabled);
			};

		auto lidar_checkbox_component = Checkbox(&lidar_status, &lidar_enabled, lidar_checkbox_option);

		lidar_signal.connect([&](bool connected)
			{
				if (connected) {
					this->car_system->startLidarDevice();
					lidar_status = LIDAR_ENABLED_MESSAGE;
					lidar_loading_debounce = false;
				}
				else {
					this->car_system->stopLidarDevice();
					lidar_status = LIDAR_DISABLED_MESSAGE;
					lidar_loading_debounce = false;
				}
			}
		);
#pragma endregion

#pragma region Wheels

#pragma region Rear Wheel Slider
		int rear_wheel_speed = 0;
		auto rear_wheel_speed_slider = Slider("Rear Wheel Speed:", &rear_wheel_speed, 0, 100, 1);
#pragma endregion

#pragma region Front Wheel Slider
		int front_wheel_angle = 90;
		auto front_wheel_angle_slider = Slider("Front Wheel Angle:", &front_wheel_angle, 0, 180, 1);
#pragma endregion
		auto slider_container = Container::Vertical({
			rear_wheel_speed_slider,
			front_wheel_angle_slider,
			}
		);

		auto wheel_settings_renderer = Renderer(slider_container, [&] {
			return
				vbox({
					rear_wheel_speed_slider->Render(),
					front_wheel_angle_slider->Render(),
					separator(),
					hbox({
						text("Rear Wheel Speed: " + std::to_string(rear_wheel_speed)) | xflex,
						separator(),
						text("Front Wheel Angle: " + std::to_string(front_wheel_angle)) | xflex,
					}),
					});
			});

#pragma endregion

#pragma endregion

		auto settings_container = Container::Vertical(
			{
				debug_checkbox_component,
				Container::Vertical(
					{
					lidar_checkbox_component,
					Renderer([&] {return separator(); }),
					wheel_settings_renderer,
					}
				) | border | Maybe(&debug_enabled)
			}
		) | border;

		settings_container |= warn_enable_debug_modal;

		int selected_tab = 0;
		std::vector<std::string> tab_titles = {
			"Main",
			"Scan Map",
			"Settings",
			"Logs",
		};

		auto tab_selection = Toggle(&tab_titles, &selected_tab);

		auto tab_content = Container::Tab(
			{
				main_component,
				renderer_line_block,
				settings_container,
			}
			, &selected_tab
		);

		auto main_container = Container::Vertical(
			{
			tab_selection,
			tab_content,
			}
		);

		auto main_renderer = Renderer(main_container, [&]
			{
				return vbox({
					text("Car Application") | bold | hcenter,
					tab_selection->Render(),
					tab_content->Render() | flex,
					}
				);
			}
		);

		std::atomic<bool> refresh_ui_continue = true;
		std::thread refresh_ui([&]
			{
				while (refresh_ui_continue) {
					using namespace std::chrono_literals;
					std::this_thread::sleep_for(0.05s);
					// The |shift| variable belong to the main thread. `screen.Post(task)`
					// will execute the update on the thread where |screen| lives (e.g. the
					// main thread). Using `screen.Post(task)` is threadsafe.
					screen.Post([&]
						{
							this->car_system->update();
						}
					);
					// After updating the state, request a new frame to be drawn. This is done
					// by simulating a new "custom" event to be handled.
					screen.Post(Event::Custom);
				}
			}
		);

		screen.Loop(main_renderer);

		this->car_system->stop();
	};
}
