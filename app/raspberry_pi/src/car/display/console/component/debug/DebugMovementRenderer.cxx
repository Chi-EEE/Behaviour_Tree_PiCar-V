#ifndef DEBUGMOVEMENTRENDERER_CXX
#define DEBUGMOVEMENTRENDERER_CXX

#pragma once

#include <nod/nod.hpp>

#include <ftxui/component/component.hpp>

using namespace ftxui;

namespace car::display::console::component::debug
{
	class DebugMovementRenderer
	{
	private:
		static constexpr int DEFAULT_REAR_WHEEL_SPEED = 0;
		static constexpr int DEFAULT_FRONT_WHEEL_ANGLE = 90;

		static constexpr auto REAR_WHEEL_DIRECTION_FORWARD_MESSAGE = "Rear Wheel Direction: Forward";
		static constexpr auto REAR_WHEEL_DIRECTION_BACKWARD_MESSAGE = "Rear Wheel Direction: Backward";

	public:
		DebugMovementRenderer()
		{
			this->rear_wheel_speed_slider = Slider("Rear Wheels Speed:", &this->rear_wheels_speed_slider_value, 0, 100, 1);
			this->rear_left_wheel_speed_slider = Slider("Left Rear Wheel Speed:", &this->rear_left_wheel_speed_slider_value, 0, 100, 1);
			this->rear_right_wheel_speed_slider = Slider("Right Rear Wheel Speed:", &this->rear_right_wheel_speed_slider_value, 0, 100, 1);

			this->front_wheels_angle_slider = Slider("Front Wheels Angle:", &this->front_wheels_angle_slider_value, 0, 180, 1);
			this->camera_servo_1_angle_slider = Slider("Camera Servo 1 Angle:", &this->camera_servo_1_angle_slider_angle, 0, 180, 1);
			this->camera_servo_2_angle_slider = Slider("Camera Servo 2 Angle:", &this->camera_servo_2_angle_slider_angle, 0, 180, 1);

			this->rear_wheel_menu_entry = MenuEntry("Rear Wheel:") | bold;
			this->servo_menu_entry = MenuEntry("Servo:") | bold;

			auto rear_wheel_direction_checkbox_option = CheckboxOption::Simple();
			rear_wheel_direction_checkbox_option.on_change = [&]
			{
				if (this->rear_wheel_direction_debounce)
				{
					this->rear_wheel_direction = !this->rear_wheel_direction;
					return;
				}
				this->rear_wheel_direction_debounce = true;
				if (this->rear_wheel_direction)
				{
					this->rear_wheel_direction_status = REAR_WHEEL_DIRECTION_FORWARD_MESSAGE;
				}
				else
				{
					this->rear_wheel_direction_status = REAR_WHEEL_DIRECTION_BACKWARD_MESSAGE;
				}
				this->rear_wheel_direction_signal(this->rear_wheel_direction);
				this->rear_wheel_direction_debounce = false;
			};

			this->rear_wheel_direction_checkbox_component = Checkbox(&this->rear_wheel_direction_status, &this->rear_wheel_direction, rear_wheel_direction_checkbox_option);

			this->slider_container =
				Container::Horizontal({Container::Vertical({
										   this->rear_wheel_menu_entry,
										   this->rear_wheel_speed_slider,
										   this->rear_left_wheel_speed_slider,
										   this->rear_right_wheel_speed_slider,
										   this->rear_wheel_direction_checkbox_component,
									   }),
									   Container::Vertical({
										   this->servo_menu_entry,
										   this->front_wheels_angle_slider,
										   this->camera_servo_1_angle_slider,
										   this->camera_servo_2_angle_slider,
									   })});
		}

		ftxui::Component element()
		{
			return Renderer(this->slider_container, [&]
							{ return hbox({
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
									  this->servo_menu_entry->Render(),
									  separator(),
									  this->front_wheels_angle_slider->Render(),
									  separator(),
									  this->camera_servo_1_angle_slider->Render(),
									  this->camera_servo_2_angle_slider->Render(),
									  separator(),
									  text("Front Wheels Angle: " + std::to_string(this->front_wheels_angle_slider_value)),
									  separator(),
									  text("Camera Servo 1 Angle: " + std::to_string(this->camera_servo_1_angle_slider_angle)),
									  text("Camera Servo 2 Angle: " + std::to_string(this->camera_servo_2_angle_slider_angle)),
								  }) | xflex,
								  separator(),
							  }); });
		}

		bool updateFrontWheels()
		{
			if (this->previous_front_wheels_angle_slider_value != this->front_wheels_angle_slider_value)
			{
				this->previous_front_wheels_angle_slider_value = this->front_wheels_angle_slider_value;
				return true;
			}
			return false;
		}

		bool updateCameraServo1()
		{
			if (this->previous_camera_servo_1_angle_slider_angle != this->camera_servo_1_angle_slider_angle)
			{
				this->previous_camera_servo_1_angle_slider_angle = this->camera_servo_1_angle_slider_angle;
				return true;
			}
			return false;
		}

		bool updateCameraServo2()
		{
			if (this->previous_camera_servo_2_angle_slider_angle != this->camera_servo_2_angle_slider_angle)
			{
				this->previous_camera_servo_2_angle_slider_angle = this->camera_servo_2_angle_slider_angle;
				return true;
			}
			return false;
		}

		bool updateRearWheels()
		{
			if (this->previous_rear_wheels_speed_slider_value != this->rear_wheels_speed_slider_value)
			{
				this->previous_rear_wheels_speed_slider_value = this->rear_wheels_speed_slider_value;

				this->previous_rear_left_wheel_speed_slider_value = this->rear_wheels_speed_slider_value;
				this->rear_left_wheel_speed_slider_value = this->rear_wheels_speed_slider_value;

				this->previous_rear_right_wheel_speed_slider_value = this->rear_wheels_speed_slider_value;
				this->rear_right_wheel_speed_slider_value = this->rear_wheels_speed_slider_value;
				return true;
			}
			if (this->previous_rear_left_wheel_speed_slider_value != this->rear_left_wheel_speed_slider_value)
			{
				this->previous_rear_left_wheel_speed_slider_value = this->rear_left_wheel_speed_slider_value;
				return true;
			}
			if (this->previous_rear_right_wheel_speed_slider_value != this->rear_right_wheel_speed_slider_value)
			{
				this->previous_rear_right_wheel_speed_slider_value = this->rear_right_wheel_speed_slider_value;
				return true;
			}
			return false;
		}

		nod::signal<void(bool)> &getRearWheelDirectionSignal()
		{
			return this->rear_wheel_direction_signal;
		}

		const int &getFrontWheelsAngleSliderValue() const
		{
			return this->front_wheels_angle_slider_value;
		}

		const int &getCameraServo1AngleSliderValue() const
		{
			return this->camera_servo_1_angle_slider_angle;
		}

		const int &getCameraServo2AngleSliderValue() const
		{
			return this->camera_servo_2_angle_slider_angle;
		}

		const int &getRearLeftWheelSpeedSliderValue() const
		{
			return this->rear_left_wheel_speed_slider_value;
		}

		const int &getRearRightWheelSpeedSliderValue() const
		{
			return this->rear_right_wheel_speed_slider_value;
		}

	private:
		nod::signal<void(bool)> rear_wheel_direction_signal;

		int previous_rear_wheels_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;
		int rear_wheels_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;

		int previous_rear_left_wheel_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;
		int rear_left_wheel_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;

		int previous_rear_right_wheel_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;
		int rear_right_wheel_speed_slider_value = DEFAULT_REAR_WHEEL_SPEED;

		int previous_front_wheels_angle_slider_value = DEFAULT_FRONT_WHEEL_ANGLE;
		int front_wheels_angle_slider_value = DEFAULT_FRONT_WHEEL_ANGLE;

		int previous_camera_servo_1_angle_slider_angle = DEFAULT_FRONT_WHEEL_ANGLE;
		int camera_servo_1_angle_slider_angle = DEFAULT_FRONT_WHEEL_ANGLE;

		int previous_camera_servo_2_angle_slider_angle = DEFAULT_FRONT_WHEEL_ANGLE;
		int camera_servo_2_angle_slider_angle = DEFAULT_FRONT_WHEEL_ANGLE;

		bool rear_wheel_direction_debounce = false;
		std::string rear_wheel_direction_status = REAR_WHEEL_DIRECTION_BACKWARD_MESSAGE;
		bool rear_wheel_direction = false;

		Component rear_wheel_speed_slider;
		Component rear_left_wheel_speed_slider;
		Component rear_right_wheel_speed_slider;

		Component rear_wheel_direction_checkbox_component;

		Component front_wheels_angle_slider;

		Component camera_servo_1_angle_slider;
		Component camera_servo_2_angle_slider;

		Component rear_wheel_menu_entry;
		Component servo_menu_entry;

		Component slider_container;
	};
}

#endif