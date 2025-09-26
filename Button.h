#pragma once
#include "Export.h"
#include "ControllerType.h"

namespace la {
	class LA_CONTROLLER_API Button {
	private:
		static constexpr unsigned int maxButtons {32};
	public:
		[[nodiscard]] bool isPressed(la::ControllerType::Dualsense button);
		[[nodiscard]] bool isPressed(la::ControllerType::Xbox button);
	};
}
