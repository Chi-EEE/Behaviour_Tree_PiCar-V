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

#pragma region Lidar Scanner Motor Checkbox
		static constexpr auto LIDAR_MOTOR_ENABLED_MESSAGE = "Lidar Motor Status: Enabled";
		static constexpr auto LIDAR_MOTOR_DISABLED_MESSAGE = "Lidar Motor Status: Disconnected";
		static constexpr auto LIDAR_MOTOR_WAIT_ENABLED_MESSAGE = "Lidar Motor Status: Enabling...";
		static constexpr auto LIDAR_MOTOR_WAIT_DISABLED_MESSAGE = "Lidar Motor Status: Disabling...";

		nod::signal<void(bool)> lidar_motor_signal;

		bool lidar_motor_loading_debounce = false;
		std::string lidar_motor_status = LIDAR_MOTOR_DISABLED_MESSAGE;
		bool lidar_motor_enabled = false;
		auto lidar_motor_checkbox_option = CheckboxOption::Simple();
		lidar_motor_checkbox_option.on_change = [&]
			{
				if (lidar_motor_loading_debounce) {
					lidar_motor_enabled = !lidar_motor_enabled;
					return;
				}
				lidar_motor_loading_debounce = true;
				if (lidar_motor_enabled) {
					lidar_motor_status = LIDAR_MOTOR_WAIT_ENABLED_MESSAGE;
				}
				else {
					lidar_motor_status = LIDAR_MOTOR_WAIT_DISABLED_MESSAGE;
				}
				lidar_motor_signal(lidar_motor_enabled);
			};

		lidar_motor_signal.connect([&](bool connected)
			{
				if (connected) {
					this->car_system->startLidarDevice();
					lidar_motor_status = LIDAR_MOTOR_ENABLED_MESSAGE;
				}
				else {
					this->car_system->stopLidarDevice();
					lidar_motor_status = LIDAR_MOTOR_DISABLED_MESSAGE;
				}
				lidar_motor_loading_debounce = false;
			}
		);

		auto lidar_motor_checkbox_component = Checkbox(&lidar_motor_status, &lidar_motor_enabled, lidar_motor_checkbox_option);
#pragma endregion

#pragma region Wheels
		static constexpr int DEFAULT_REAR_WHEEL_SPEED = 0;
		static constexpr int DEFAULT_FRONT_WHEEL_ANGLE = 90;

		int previous_rear_wheels_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;

		int rear_wheels_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;
		int rear_left_wheel_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;
		int rear_right_wheel_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;

		auto rear_wheel_speed_slider = Slider("Rear Wheels Speed:", &rear_wheels_speed_slider_value, 0, 100, 1);
		auto rear_left_wheel_speed_slider = Slider("Left Rear Wheel Speed:", &rear_left_wheel_speed_slider_value, 0, 100, 1);
		auto rear_right_wheel_speed_slider = Slider("Right Rear Wheel Speed:", &rear_right_wheel_speed_slider_value, 0, 100, 1);

		static constexpr auto REAR_WHEEL_DIRECTION_FORWARD_MESSAGE = "Rear Wheel Direction: Forward";
		static constexpr auto REAR_WHEEL_DIRECTION_BACKWARD_MESSAGE = "Rear Wheel Direction: Backward";

		nod::signal<void(bool)> rear_wheel_direction_signal;

		bool rear_wheel_direction_debounce = false;
		std::string rear_wheel_direction_status = LIDAR_MOTOR_DISABLED_MESSAGE;
		bool rear_wheel_direction = true;
		auto rear_wheel_direction_checkbox_option = CheckboxOption::Simple();
		rear_wheel_direction_checkbox_option.on_change = [&]
			{
				if (rear_wheel_direction_debounce) {
					rear_wheel_direction = !rear_wheel_direction;
					return;
				}
				rear_wheel_direction_debounce = true;
				if (rear_wheel_direction) {
					rear_wheel_direction_status = REAR_WHEEL_DIRECTION_FORWARD_MESSAGE;
				}
				else {
					rear_wheel_direction_status = REAR_WHEEL_DIRECTION_BACKWARD_MESSAGE;
				}
				rear_wheel_direction_signal(rear_wheel_direction);
			};

		rear_wheel_direction_signal.connect([&](bool direction)
			{
				if (direction) {
					this->car_system->setRearWheelDirectionToForwards();
					rear_wheel_direction_status = REAR_WHEEL_DIRECTION_FORWARD_MESSAGE;
				}
				else {
					this->car_system->setRearWheelDirectionToBackwards();
					rear_wheel_direction_status = REAR_WHEEL_DIRECTION_BACKWARD_MESSAGE;
				}
				rear_wheel_direction_debounce = false;
			}
		);
		auto rear_wheel_direction_checkbox_component = Checkbox(&rear_wheel_direction_status, &rear_wheel_direction, rear_wheel_direction_checkbox_option);

		int previous_front_wheels_angle_slider_value = DEFAULT_FRONT_WHEEL_ANGLE;

		int front_wheels_angle_slider_value = DEFAULT_FRONT_WHEEL_ANGLE;
		int front_left_wheel_angle_slider_value = DEFAULT_FRONT_WHEEL_ANGLE;
		int front_right_wheel_angle_slider_value = DEFAULT_FRONT_WHEEL_ANGLE;

		auto front_wheels_angle_slider = Slider("Front Wheels Angle:", &front_wheels_angle_slider_value, 0, 180, 1);
		auto front_left_wheel_angle_slider = Slider("Left Front Wheel Angle:", &front_left_wheel_angle_slider_value, 0, 180, 1);
		auto front_right_wheel_angle_slider = Slider("Right Front Wheel Angle:", &front_right_wheel_angle_slider_value, 0, 180, 1);

		auto rear_wheel_menu_entry = MenuEntry("Rear Wheel:") | bold;
		auto front_wheel_menu_entry = MenuEntry("Front Wheel:") | bold;

		auto slider_container =
			Container::Horizontal({
				Container::Vertical({
					rear_wheel_menu_entry,
					rear_wheel_speed_slider,
					rear_left_wheel_speed_slider,
					rear_right_wheel_speed_slider,
					rear_wheel_direction_checkbox_component,
				}),
				Container::Vertical({
					front_wheel_menu_entry,
					front_wheels_angle_slider,
					front_left_wheel_angle_slider,
					front_right_wheel_angle_slider,
				})
			});

		auto wheel_settings_renderer = Renderer(slider_container, [&] {
			return
				hbox({
					vbox({
						rear_wheel_menu_entry->Render(),
						separator(),
						rear_wheel_speed_slider->Render(),
						separator(),
						rear_left_wheel_speed_slider->Render(),
						rear_right_wheel_speed_slider->Render(),
						separator(),
						text("Left Rear Wheel Speed: " + std::to_string(rear_left_wheel_speed_slider_value)),
						text("Right Rear Wheel Speed: " + std::to_string(rear_right_wheel_speed_slider_value)),
						separator(),
						rear_wheel_direction_checkbox_component->Render(),
					}) | xflex,
					separator(),
					vbox({
						front_wheel_menu_entry->Render(),
						separator(),
						front_wheels_angle_slider->Render(),
						separator(),
						front_left_wheel_angle_slider->Render(),
						front_right_wheel_angle_slider->Render(),
						separator(),
						text("Left Front Wheel Angle: " + std::to_string(front_left_wheel_angle_slider_value)),
						text("Right Front Wheel Angle: " + std::to_string(front_right_wheel_angle_slider_value)),
					}) | xflex,
					separator(),
					});
			});

#pragma endregion

#pragma endregion

		auto settings_container = Container::Vertical(
			{
				debug_checkbox_component,
				Container::Vertical(
					{
						lidar_motor_checkbox_component,
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
							if (previous_front_wheels_angle_slider_value != front_wheels_angle_slider_value) {
								previous_front_wheels_angle_slider_value = front_wheels_angle_slider_value;
								front_left_wheel_angle_slider_value = front_wheels_angle_slider_value;
								front_right_wheel_angle_slider_value = front_wheels_angle_slider_value;
								this->car_system->setFrontLeftWheelAngle({ front_left_wheel_angle_slider_value * 1.0f });
								this->car_system->setFrontRightWheelAngle({ front_right_wheel_angle_slider_value * 1.0f });
							}
							if (previous_rear_wheels_speed_slider_value != rear_wheels_speed_slider_value) {
								previous_rear_wheels_speed_slider_value = rear_wheels_speed_slider_value;
								rear_left_wheel_speed_slider_value = rear_wheels_speed_slider_value;
								rear_right_wheel_speed_slider_value = rear_wheels_speed_slider_value;
								this->car_system->setRearLeftWheelSpeed({ rear_left_wheel_speed_slider_value });
								this->car_system->setRearRightWheelSpeed({ rear_right_wheel_speed_slider_value });
							}
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
