#ifndef DEBUGWHEELRENDERER_CXX
#define DEBUGWHEELRENDERER_CXX

#pragma once

#include <nod/nod.hpp>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/loop.hpp> // ftxui::Loop

using namespace ftxui;

namespace car::display::component {
	class DebugWheelRenderer {
	private:
		static constexpr int DEFAULT_REAR_WHEEL_SPEED = 0;
		static constexpr int DEFAULT_FRONT_WHEEL_ANGLE = 90;

		static constexpr auto REAR_WHEEL_DIRECTION_FORWARD_MESSAGE = "Rear Wheel Direction: Forward";
		static constexpr auto REAR_WHEEL_DIRECTION_BACKWARD_MESSAGE = "Rear Wheel Direction: Backward";
	public:
		DebugWheelRenderer() {

		}

		ftxui::Component element()
		{
			this->rear_wheel_speed_slider = Slider("Rear Wheels Speed:", &this->rear_wheels_speed_slider_value, 0, 100, 1);
			this->rear_left_wheel_speed_slider = Slider("Left Rear Wheel Speed:", &this->rear_left_wheel_speed_slider_value, 0, 100, 1);
			this->rear_right_wheel_speed_slider = Slider("Right Rear Wheel Speed:", &this->rear_right_wheel_speed_slider_value, 0, 100, 1);

			this->front_wheels_angle_slider = Slider("Front Wheels Angle:", &this->front_wheels_angle_slider_value, 0, 180, 1);
			this->front_left_wheel_angle_slider = Slider("Left Front Wheel Angle:", &this->front_left_wheel_angle_slider_value, 0, 180, 1);
			this->front_right_wheel_angle_slider = Slider("Right Front Wheel Angle:", &this->front_right_wheel_angle_slider_value, 0, 180, 1);

			this->rear_wheel_menu_entry = MenuEntry("Rear Wheel:") | bold;
			this->front_wheel_menu_entry = MenuEntry("Front Wheel:") | bold;

			auto rear_wheel_direction_checkbox_option = CheckboxOption::Simple();
			rear_wheel_direction_checkbox_option.on_change = [&]
				{
					if (this->rear_wheel_direction_debounce) {
						this->rear_wheel_direction = !this->rear_wheel_direction;
						return;
					}
					this->rear_wheel_direction_debounce = true;
					if (this->rear_wheel_direction) {
						this->rear_wheel_direction_status = REAR_WHEEL_DIRECTION_FORWARD_MESSAGE;
					}
					else {
						this->rear_wheel_direction_status = REAR_WHEEL_DIRECTION_BACKWARD_MESSAGE;
					}
					this->rear_wheel_direction_signal(this->rear_wheel_direction);
					this->rear_wheel_direction_debounce = false;
				};

			this->rear_wheel_direction_checkbox_component = Checkbox(&this->rear_wheel_direction_status, &this->rear_wheel_direction, rear_wheel_direction_checkbox_option);

			this->slider_container =
				Container::Horizontal({
					Container::Vertical({
						this->rear_wheel_menu_entry,
						this->rear_wheel_speed_slider,
						this->rear_left_wheel_speed_slider,
						this->rear_right_wheel_speed_slider,
						this->rear_wheel_direction_checkbox_component,
					}),
					Container::Vertical({
						this->front_wheel_menu_entry,
						this->front_wheels_angle_slider,
						this->front_left_wheel_angle_slider,
						this->front_right_wheel_angle_slider,
					})
					}
			);

			return Renderer(this->slider_container, [&] {
				return
					hbox({
						vbox({
							this->rear_wheel_menu_entry->Render(),
							separator(),
							this->rear_wheel_speed_slider->Render(),
							separator(),
							this->rear_left_wheel_speed_slider->Render(),
							this->rear_right_wheel_speed_slider->Render(),
							separator(),
							text("Left Rear Wheel Speed: " + std::to_string(this->rear_left_wheel_speed_slider_value)),
							text("Right Rear Wheel Speed: " + std::to_string(this->rear_right_wheel_speed_slider_value)),
							separator(),
							this->rear_wheel_direction_checkbox_component->Render(),
						}) | xflex,
						separator(),
						vbox({
							this->front_wheel_menu_entry->Render(),
							separator(),
							this->front_wheels_angle_slider->Render(),
							separator(),
							this->front_left_wheel_angle_slider->Render(),
							this->front_right_wheel_angle_slider->Render(),
							separator(),
							text("Left Front Wheel Angle: " + std::to_string(this->front_left_wheel_angle_slider_value)),
							text("Right Front Wheel Angle: " + std::to_string(this->front_right_wheel_angle_slider_value)),
						}) | xflex,
						separator(),
						});
				});
		}

		bool updateFrontWheels() {
			if (this->previous_front_wheels_angle_slider_value != this->front_wheels_angle_slider_value) {
				this->previous_front_wheels_angle_slider_value = this->front_wheels_angle_slider_value;
				this->front_left_wheel_angle_slider_value = this->front_wheels_angle_slider_value;
				this->front_right_wheel_angle_slider_value = this->front_wheels_angle_slider_value;
				return true;
			}
			return false;
		}

		bool updateRearWheels() {
			if (this->previous_rear_wheels_speed_slider_value != this->rear_wheels_speed_slider_value) {
				this->previous_rear_wheels_speed_slider_value = this->rear_wheels_speed_slider_value;
				this->rear_left_wheel_speed_slider_value = this->rear_wheels_speed_slider_value;
				this->rear_right_wheel_speed_slider_value = this->rear_wheels_speed_slider_value;
				return true;
			}
			return false;
		}

		nod::signal<void(bool)>& getRearWheelDirectionSignal() {
			return this->rear_wheel_direction_signal;
		}

		const int& getFrontLeftWheelAngleSliderValue() const {
			return this->front_left_wheel_angle_slider_value;
		}

		const int& getFrontRightWheelAngleSliderValue() const {
			return this->front_right_wheel_angle_slider_value;
		}

		const int& getRearLeftWheelSpeedSliderValue() const {
			return this->rear_left_wheel_speed_slider_value;
		}

		const int& getRearRightWheelSpeedSliderValue() const {
			return this->rear_right_wheel_speed_slider_value;
		}

	private:
		nod::signal<void(bool)> rear_wheel_direction_signal;

		int previous_rear_wheels_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;
		int rear_wheels_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;
		int rear_left_wheel_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;
		int rear_right_wheel_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;

		int previous_front_wheels_angle_slider_value = DEFAULT_FRONT_WHEEL_ANGLE;
		int front_wheels_angle_slider_value = DEFAULT_FRONT_WHEEL_ANGLE;
		int front_left_wheel_angle_slider_value = DEFAULT_FRONT_WHEEL_ANGLE;
		int front_right_wheel_angle_slider_value = DEFAULT_FRONT_WHEEL_ANGLE;

		bool rear_wheel_direction_debounce = false;
		std::string rear_wheel_direction_status = REAR_WHEEL_DIRECTION_FORWARD_MESSAGE;
		bool rear_wheel_direction = true;

		Component rear_wheel_speed_slider;
		Component rear_left_wheel_speed_slider;
		Component rear_right_wheel_speed_slider;

		Component rear_wheel_direction_checkbox_component;

		Component front_wheels_angle_slider;
		Component front_left_wheel_angle_slider;
		Component front_right_wheel_angle_slider;

		Component rear_wheel_menu_entry;
		Component front_wheel_menu_entry;

		Component slider_container;
	};
}

#endif