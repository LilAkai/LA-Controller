#include "LA-Controller/Controller.h"
#include <iostream>

using dualsense = la::ControllerType::Dualsense;

void main() {
	la::Init();

	la::Controller ctrl = la::Controller();

	while (1) {
		ctrl.autoDetectAndConnect();

		if (ctrl.isConnected()) {
			if (ctrl.button.isPressed(dualsense::Cross)) {
				std::cout<<"cross pressed"<<std::endl;
			}
		}
	}

	ctrl.disconnectController();
}