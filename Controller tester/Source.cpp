#include <iostream>
#include <unordered_map>

#include "LA-Controller/Controller.h"

using dualsense = la::ControllerType::Dualsense;
using xbox = la::ControllerType::Xbox;

void main() {
    if (la::Init())
        return;

    la::Controller ctrl;
    ctrl.autoDetectAndConnect();

    // Map DualSense
    const std::unordered_map<dualsense, std::string> dualsenseButtons = {
        {dualsense::Cross, "Cross"},
        {dualsense::Circle, "Circle"},
        {dualsense::Square, "Square"},
        {dualsense::Triangle, "Triangle"},
        {dualsense::Options, "Options"},
        {dualsense::PS, "PS"},
        {dualsense::TouchPad, "TouchPad"},
        {dualsense::Mute, "Mute"},
        {dualsense::Share, "Share"},
        {dualsense::L1, "L1"},
        {dualsense::R1, "R1"},
        {dualsense::LStick_DS, "Left Stick"},
        {dualsense::RStick_DS, "Right Stick"}
    };

    // Map Xbox
    const std::unordered_map<xbox, std::string> xboxButtons = {
        {xbox::A, "A"},
        {xbox::B, "B"},
        {xbox::X, "X"},
        {xbox::Y, "Y"},
        {xbox::Start, "Start"},
        {xbox::Home, "Xbox"},
        {xbox::Select, "Select"},
        {xbox::LB, "LB"},
        {xbox::RB, "RB"},
        {xbox::LStick_Xbox, "Left Stick"},
        {xbox::RStick_Xbox, "Right Stick"}
    };

    while (true) {
        if (ctrl.isConnected()) {
            ctrl.updateInputs();

            if (ctrl.isDualSense()) {
                for (const auto &[button, name]:dualsenseButtons) {
                    if (ctrl.button.isPressed(button)) {
                        std::cout<<"[DualSense] "<<name<<" pressed\n";
                    }
                }
                if (!ctrl.dpad.isNeutral()) {
                    std::cout<<"Dpad direction: "<<static_cast<int>(ctrl.dpad.getDirection())<<"\n";
                }
                if (ctrl.leftJoystick.getAxis(0).length()>0.1f) {
                    std::cout<<"Left Stick: ("<<ctrl.leftJoystick.getAxis(0).x<<", "<<ctrl.leftJoystick.getAxis(0).y<<")\n";
				}
                if (ctrl.rightJoystick.getAxis(1).length()>0.1f) {
                    std::cout<<"Right Stick: ("<<ctrl.rightJoystick.getAxis(1).x<<", "<<ctrl.rightJoystick.getAxis(1).y<<")\n";
                }

            } else if (ctrl.isXboxController()) {
                for (const auto &[button, name]:xboxButtons) {
                    if (ctrl.button.isPressed(button)) {
                        std::cout<<"[Xbox] "<<name<<" pressed\n";
                    }
                }
                if (!ctrl.dpad.isNeutral()) {
                    std::cout<<"Dpad direction: "<<static_cast<int>(ctrl.dpad.getDirection())<<"\n";
                }
                if (ctrl.leftJoystick.getAxis(0).length()>0.1f) {
                    std::cout<<"Left Stick: ("<<ctrl.leftJoystick.getAxis(0).x<<", "<<ctrl.leftJoystick.getAxis(0).y<<")\n";
                }
                if (ctrl.rightJoystick.getAxis(1).length()>0.1f) {
                    std::cout<<"Right Stick: ("<<ctrl.rightJoystick.getAxis(1).x<<", "<<ctrl.rightJoystick.getAxis(1).y<<")\n";
                }

            } else {
                std::cout<<"Manette non reconnue\n";
            }
        } else if (!ctrl.isConnected()) {
			ctrl.autoDetectAndConnect();
        }
    }

    ctrl.disconnectController();
}