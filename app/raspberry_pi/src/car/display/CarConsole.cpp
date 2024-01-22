#include "CarConsole.h"

namespace car::display {
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

		bool exit_modal_shown = false;

		auto show_exit_modal = [&] { exit_modal_shown = true; };
		auto hide_exit_modal = [&] { exit_modal_shown = false; };

		auto exit = screen.ExitLoopClosure();

		auto modal_component = ExitModalComponent(hide_exit_modal, exit);

		Box box;
		auto left_side =
			Container::Vertical({
				Renderer([&]() {
					// TODO: Fix the y size of the canvas
					auto c = Canvas(box.x_max, box.y_max * 3);
					for (auto& point : this->car_system->getScanData()) {
						const double angle = point.angle;
						const double distance = point.distance;
						const double angleInRadians = angle * (3.14159265f / 180.0f);
						const int x = distance * std::cos(angleInRadians);
						const int y = distance * std::sin(angleInRadians);
						c.DrawBlockLine(50, 50, x, y, Color::Blue);
					}
					return canvas(std::move(c));
				}),
				Renderer([&] {return separator(); }),
			Container::Horizontal({
				main_button | center,
				Renderer([&] {return separator(); }),
				Button("Quit", show_exit_modal, animated_button_style) | center,
			}),
				}
		) | xflex;

		auto right_side =
			Container::Vertical({
					Renderer([&] { return text("Behaviour Tree Queue"); }),
					Renderer([&] { return separator(); }),
					Container::Vertical({
					}),
				}
		);

		auto main_component = Container::Horizontal(
			{
				left_side,
				Renderer([&] {return separator(); }),
				right_side,
			}
		) | border | flex | reflect(box);

		main_component |= Modal(modal_component, &exit_modal_shown);
#pragma endregion

		SettingsScreen settings_screen(this->car_system);
		auto settings_container = settings_screen.element();

		//LoggingScreen logging_screen;
		//auto logging_container = logging_screen.element();

		int selected_tab = 0;
		std::vector<std::string> tab_titles = {
			"Main",
			"Settings",
			"Logs",
		};

		auto tab_selection = Toggle(&tab_titles, &selected_tab);

		ftxui::Elements line_elements;
		auto tab_content = Container::Tab(
			{
				main_component,
				settings_container,
				Renderer(
					[&] {
						// TODO: Improve this so it doesn't create a log of lines
						line_elements.clear();
						for (const std::string& message : this->vector_sink->get_log_messages())
						{
							line_elements.push_back(paragraph(message));
							line_elements.push_back(separator());
						}
						// TODO: Make the log scrollable
						return vbox(line_elements) | xflex | vscroll_indicator | frame;
					}
				)
			}
		, &selected_tab);

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
					tab_selection->Render() | flex | center,
					tab_content->Render() | xflex ,
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
					screen.Post([&]
						{
							settings_screen.update();
						}
					);
					this->car_system->update();
					screen.Post(Event::Custom);
				}
			}
		);

		std::thread screen_thread([&]
			{
				screen.Loop(main_renderer);
			}
		);
		refresh_ui.detach();
		screen_thread.join();

		spdlog::info("Exiting CarConsole::run()");
		this->car_system->stop();
	};
}
