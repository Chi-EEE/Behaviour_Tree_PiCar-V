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

	CarConsole::CarConsole(std::shared_ptr<CarSystem> car_system, std::shared_ptr<logging::vector_sink_mt> vector_sink) : car_system(std::move(car_system)), vector_sink(vector_sink) {
		vector_sink->get_log_messages();
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
		// TODO: Fix crash which happens with this.
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
		SettingsScreen settings_screen(this->car_system);
		auto settings_container = settings_screen.element();

		//LoggingScreen logging_screen;
		//auto logging_container = logging_screen.element();

		int selected_tab = 0;
		std::vector<std::string> tab_titles = {
			"Main",
			"Scan Map",
			"Settings",
			"Logs",
		};

		auto tab_selection = Toggle(&tab_titles, &selected_tab);

		ftxui::Elements line_elements;
		auto tab_content = Container::Tab(
			{
				main_component,
				renderer_line_block,
				settings_container,
				Renderer(
					[&] {
						line_elements.clear();
						for (const std::string& message : this->vector_sink->get_log_messages())
						{
							line_elements.push_back(paragraph(message));
						}
						return vbox(line_elements) | xflex;
					}
				)
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
							settings_screen.update();
						}
					);
					this->car_system->update();
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
