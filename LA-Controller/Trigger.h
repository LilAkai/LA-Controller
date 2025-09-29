#pragma once
#include "Export.h"

namespace la {
	class LA_CONTROLLER_API Trigger {
	private:
		float value;
	public:
		// Constructeur par défaut
		Trigger();
		float getValue();
	};
}