#ifndef DUMMYWHEELCONTROLLER_H
#define DUMMYWHEELCONTROLLER_H

#pragma once

#include "AbstractMovementController.h"

namespace car::system::movement::controller
{
	class DummyMovementController : public AbstractMovementController
	{
	public:
		void initialize() final override {};

		void stop() final override;

		void terminate() final override {};

		void setRearWheelsSpeed(const int speed) final override;

		void setRearLeftWheelSpeed(const int speed) final override;

		void setRearRightWheelSpeed(const int speed) final override;

		void setFrontWheelsAngle(const float angle) final override;

		void setCameraServo1Angle(const float angle) final override;

		void setCameraServo2Angle(const float angle) final override;

		void setRearWheelsDirectionToForward() final override;

		void setRearLeftWheelDirectionToForward() final override;

		void setRearRightWheelDirectionToForward() final override;

		void setRearWheelsDirectionToBackward() final override;

		void setRearLeftWheelDirectionToBackward() final override;

		void setRearRightWheelDirectionToBackward() final override;

	private:
	};
} // namespace car::system::movement::controller

#endif