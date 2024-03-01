#include "CarConsole.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/loop.hpp>

#include <nod/nod.hpp>

using namespace ftxui;

namespace car::display::console
{
	CarConsole::CarConsole(std::shared_ptr<CarSystem> car_system, std::shared_ptr<JsonConfiguration> json_configuration, std::shared_ptr<logging::vector_sink_mt> vector_sink) : car_system(std::move(car_system)), json_configuration(std::move(json_configuration)), vector_sink(vector_sink) {};

	void CarConsole::initialize()
	{
		this->car_system->initialize();
	};

	void CarConsole::run()
	{
		ScreenInteractive screen = ScreenInteractive::Fullscreen();

		// Have a bool to exit the screen after it finishes updating the UI
		std::atomic_bool exit_screen = false;
		std::function<void()> exit = [&]() {
			exit_screen.store(true);
			};

		MainScreen main_screen(this->car_system, exit);
		auto main_screen_container = main_screen.element();
		
		SettingsScreen settings_screen(this->car_system, this->json_configuration);
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
				do {
					using namespace std::chrono_literals;
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
					screen.Post([&]
						{
							settings_screen.update();
						}
					);
					this->car_system->update();
					screen.Post(Event::Custom);
					if (exit_screen) {
						screen.ExitLoopClosure()();
						break;
					}
				} while (true);
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
