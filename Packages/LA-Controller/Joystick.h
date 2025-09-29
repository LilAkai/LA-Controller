#pragma once
#include "Export.h"
#include "Vector.h"

namespace la {
	class LA_CONTROLLER_API Joystick {
	private:
		la::Vector2f axis;
		static constexpr unsigned int axisCount {8};
	public:
		// Constructeur par défaut
		Joystick();
		la::Vector2f getAxis();
	};
}

