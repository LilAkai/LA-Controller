#pragma once
#include "Export.h"

namespace la {
	class Controller;


	class LA_CONTROLLER_API Trigger {
	private:
		Controller &parent;
		float value;


	private:
		void readHIDAxisData(unsigned int index);


	public:
		Trigger() = default;
		Trigger(Controller &parent);
		~Trigger() = default;


		float getValue();
		void updateState(unsigned int index);
	};
}