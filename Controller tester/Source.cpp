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
				if (ctrl.button.isPressed(dualsense::PS)) {
					std::cout<<"PS button pressed\n";
				}
				if (ctrl.button.isPressed(dualsense::TouchPad)) {
					std::cout<<"TouchPad pressed\n";
				}
				if (ctrl.button.isPressed(dualsense::Mute)) {
					std::cout<<"Mute button pressed\n";
				}
				if (ctrl.button.isPressed(dualsense::Share)) {
					std::cout<<"Share button pressed\n";
				}
				if (ctrl.button.isPressed(dualsense::L1)) {
					std::cout<<"L1 pressed\n";
				}
				if (ctrl.button.isPressed(dualsense::R1)) {
					std::cout<<"R1 pressed\n";
				}
				if (ctrl.button.isPressed(dualsense::LStick_DS)) {
					std::cout<<"Left Stick pressed\n";
				}
				if (ctrl.button.isPressed(dualsense::RStick_DS)) {
					std::cout<<"Right Stick pressed\n";
				}

				//std::cout<<"Left Stick: ("<<ctrl.leftJoystick.getAxis(0).x<<", "<<ctrl.leftJoystick.getAxis(0).y<<")\n";
				std::cout<<"Dpad direction: "<<static_cast<int>(ctrl.dpad.getDirection())<<"\n";
			}
			else if (ctrl.isXboxController()) {
			}
			else {
			}
			
		}
	}

	ctrl.disconnectController();
}