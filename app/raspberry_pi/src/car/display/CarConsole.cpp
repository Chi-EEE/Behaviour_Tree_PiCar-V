#include "CarConsole.h"

namespace car::display
{
	CarConsole::CarConsole(std::shared_ptr<CarSystem> car_system, std::shared_ptr<logging::vector_sink_mt> vector_sink) : car_system(std::move(car_system)), vector_sink(vector_sink) {};

	void CarConsole::initialize()
	{
		this->car_system->initialize();
	};

	void CarConsole::run()
	{
		std::atomic<bool> refresh_ui_continue = true;

		ScreenInteractive screen = ScreenInteractive::Fullscreen();

		std::function<void()> exit = [&]() {
			refresh_ui_continue.store(false);
			screen.ExitLoopClosure()();
			};

		MainScreen main_screen(this->car_system, exit);
		auto main_screen_container = main_screen.element();

		SettingsScreen settings_screen(this->car_system);
		auto settings_screen_container = settings_screen.element();

		LoggingScreen logging_screen(this->vector_sink);
		auto logging_container = logging_screen.element();

		int selected_tab = 0;
		std::vector<std::string> tab_titles = {
			"Main",
			"Settings",
			"Logs",
		};

		auto tab_selection = Toggle(&tab_titles, &selected_tab);

		auto tab_content = Container::Tab(
			{
				main_screen_container,
				settings_screen_container,
				logging_container,
			},
			&selected_tab);

		auto main_container = Container::Vertical(
			{
				tab_selection,
				tab_content,
			});

		auto main_renderer = Renderer(main_container, [&]
			{ return vbox({
				  text("Car Application") | bold | hcenter,
				  tab_selection->Render() | flex | center,
				  tab_content->Render() | xflex,
				}); });

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
	}


	void CarConsole::terminate()
	{
		this->car_system->terminate();
	}
}
