#include <iostream>
#include <fstream>
#include <optional>
#include <chrono> // std::chrono::microseconds
#include <thread> // std::this_thread::sleep_for
#include <string>
#include <cstdlib>

#include <fmt/format.h>

#include <nlohmann/json.hpp>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <ftxui/component/loop.hpp> // For ftxui::Loop

#include <rplidar/RPLidar.h>

#include "global/Config.hpp"

#include "car_system/CarSystem.h"
#include "car_system/lidar/LidarScanner.hpp"
#include "car_system/lidar/LidarDummy.hpp"

using json = nlohmann::json;

using namespace car_system;
using namespace car_system::lidar;

using namespace rplidar;

std::string getWebsocketUrl()
{
	std::optional<int> maybe_port = GET_CONFIG_VALUE(port);
	if (maybe_port.has_value())
	{
		return fmt::format("ws://{}:{}/ws/room?request=join&type=car&room_name={}", GET_CONFIG_VALUE(host), maybe_port.value(), GET_CONFIG_VALUE(room));
	}
	return fmt::format("ws://{}/ws/room?request=join&type=&room_name={}", GET_CONFIG_VALUE(host), GET_CONFIG_VALUE(room));
}

// Car is a global variable so that car.terminate() can be called on exit
std::unique_ptr<CarSystem> car_system_obj;

void terminate() {
	car_system_obj->terminate();
	spdlog::info("Terminated");
	system("pause");
}

auto button_style = ftxui::ButtonOption::Animated();

ftxui::Component MainComponent(std::function<void()> show_modal,
	std::function<void()> exit) {
	auto component = ftxui::Container::Vertical({
		ftxui::Button("Show modal", show_modal, button_style),
		ftxui::Button("Quit", exit, button_style),
		});
	// Polish how the two buttons are rendered:
	component |= ftxui::Renderer([&](ftxui::Element inner) {
		return ftxui::vbox({
				   ftxui::text("Main component"),
				   ftxui::separator(),
				   inner,
			})                                //
			| ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 15)   //
			| ftxui::size(ftxui::HEIGHT, ftxui::GREATER_THAN, 15)  //
			| ftxui::border                          //
			| ftxui::center;                         //
		});
	return component;
}


int main()
{
	auto screen = ftxui::ScreenInteractive::FitComponent();

	int custom_loop_count = 0;
	int frame_count = 0;
	int event_count = 0;

	auto _MainComponent = ftxui::Renderer([&] {
		frame_count++;
		return ftxui::vbox({
				   ftxui::text("This demonstrates using a custom ftxui::Loop. It "),
				   ftxui::text("runs at 100 iterations per seconds. The FTXUI events "),
				   ftxui::text("are all processed once per iteration and a new frame "),
				   ftxui::text("is rendered as needed"),
				   ftxui::separator(),
				   ftxui::text("ftxui event count: " + std::to_string(event_count)),
				   ftxui::text("ftxui frame count: " + std::to_string(frame_count)),
				   ftxui::text("Custom loop count: " + std::to_string(custom_loop_count)),
			}) |
			ftxui::border;
		});

	_MainComponent |= ftxui::CatchEvent([&](ftxui::Event) -> bool {
		event_count++;
		return false;
		});

	bool modal_shown = false;
	auto show_modal = [&] { modal_shown = true; };
	auto exit = screen.ExitLoopClosure();
	ftxui::Loop loop(&screen, MainComponent(show_modal, exit));

	while (!loop.HasQuitted()) {
		custom_loop_count++;
		loop.RunOnce();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	/*bool modal_shown = false;
	auto show_modal = [&] { modal_shown = true; };
	auto exit = screen.ExitLoopClosure();
	auto main_component = MainComponent(show_modal, exit);
	screen.Loop(main_component);*/

	// spdlog::set_level(spdlog::level::off);
	//std::string websocket_url = getWebsocketUrl();
	//spdlog::info("Got websocket url: {}", websocket_url);

	////std::unique_ptr<LidarDummy> scanner = std::make_unique<LidarDummy>();
	// std::unique_ptr<LidarScanner> scanner = std::make_unique<LidarScanner>("COM3");

	//std::unique_ptr<MessagingSystem> messaging_system = std::make_unique<MessagingSystem>(websocket_url);

	//car_system_obj = std::make_unique<CarSystem>(websocket_url, std::move(scanner), std::move(messaging_system));

	//car_system_obj->run();

	return 0;
}
