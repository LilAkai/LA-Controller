#include <iostream>

#include "LA-Controller/Controller.h"

using dualsense = la::ControllerType::Dualsense;

void main() {
	if (la::Init())
		return;

	la::Controller ctrl;
	ctrl.autoDetectAndConnect();

	while (1) {
		if (ctrl.isConnected()) {
			ctrl.updateInputs();
			if (ctrl.button.isPressed(dualsense::Cross)) {
				std::cout<<"cross pressed"<<std::endl;
			}
		}
	}

	ctrl.disconnectController();
}