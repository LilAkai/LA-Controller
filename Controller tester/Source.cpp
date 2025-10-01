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
			if (ctrl.isDualSense()) {
				std::cout<<"DualSense connected\n";



				if (ctrl.button.isPressed(dualsense::Options)) {
					std::cout<<"Options pressed\n";
				}
				if (ctrl.button.isPressed(dualsense::Cross)) {
					std::cout<<"cross pressed\n";
				} 
				if (ctrl.button.isPressed(dualsense::Circle)) {
					std::cout<<"Circle pressed\n";
				}
				if (ctrl.button.isPressed(dualsense::Square)) {
					std::cout<<"Square pressed\n";
				}
				if (ctrl.button.isPressed(dualsense::Triangle)) {
					std::cout<<"Triangle pressed\n";
				}
			}
			else if (ctrl.isXboxController()) {
				std::cout<<"Xbox controller connected\n";
			}
			else {
				std::cout<<"Unknown controller type\n";
			}
			
		}
	}

	ctrl.disconnectController();
}