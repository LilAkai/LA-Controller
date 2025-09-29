#pragma once
#include "Export.h"

namespace la {
	namespace ControllerType {
		enum Xbox {
			A = 0,
			B,
			X,
			Y,

			Start = 7,
			Select = 6,

			LB = 4,
			RB = 5,

			LStick_Xbox = 8,
			RStick_Xbox = 9
		};

		enum Dualsense {
			Cross = 0,
			Circle,
			Square,
			Triangle,

			Options = 7,
			Share = 6,

			L1 = 4,
			R1 = 5,

			LStick_DS = 8,
			RStick_DS = 9
		};
	}
}