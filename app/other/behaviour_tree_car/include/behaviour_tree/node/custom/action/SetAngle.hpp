#ifndef BEHAVIOUR_TREE_SETANGLE_HPP
#define BEHAVIOUR_TREE_SETANGLE_HPP

#pragma once

#include "behaviour_tree/node/custom/CustomNode.hpp"

#include "behaviour_tree/Context.h"
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
#include "behaviour_tree/CarContext.hpp"
#endif // !BEHAVIOUR_TREE_DISABLE_RUN

#include "ServoType.hpp"

namespace behaviour_tree::node::custom::action
{
	class SetAngle final : public CustomNode
	{
	public:
		SetAngle(const std::string& name, const ServoType& servo_type, const int& angle) : CustomNode(name), servo_type(servo_type), angle(angle)
		{
		}

		const Status run(const int& tick_count, std::shared_ptr<Context> context) final override {
#ifndef BEHAVIOUR_TREE_DISABLE_RUN
			std::shared_ptr<CarContext> car_context = std::dynamic_pointer_cast<CarContext>(context);
			auto car_system = car_context->getCarSystem();
			const MovementSystem* movement_system = car_system->getMovementSystem();
			switch (this->getServoType())
			{
			case ServoType::FrontWheels:
				movement_system->setFrontWheelsAngle(this->getAngle());
				break;
			case ServoType::CameraServo1:
				movement_system->setCameraServo1Angle(this->getAngle());
				break;
			case ServoType::CameraServo2:
				movement_system->setCameraServo2Angle(this->getAngle());
				break;
			}
#endif // !BEHAVIOUR_TREE_DISABLE_RUN
			return Status::Success;
		}

		const std::string toString() const final override {
			std::string servo_type;
			switch (this->getServoType())
			{
			case ServoType::FrontWheels:
				servo_type = "FrontWheels";
				break;
			case ServoType::CameraServo1:
				servo_type = "CameraServo1";
				break;
			case ServoType::CameraServo2:
				servo_type = "CameraServo2";
				break;
			}
			const std::string& name = this->getName();
			if (name != "")
				return fmt::format(R"(<Action:SetAngle name='{}' servo_type='{}' angle='{}'/>)", name, servo_type, this->getAngle());
			else
				return fmt::format(R"(<Action:SetAngle servo_type='{}' angle='{}'/>)", servo_type, this->getAngle());
		}

		const ServoType& getServoType() const
		{
			return this->servo_type;
		}

		const int& getAngle() const {
			return this->angle;
		}

	private:
		const int angle;
		const ServoType servo_type;
	};
}

#endif