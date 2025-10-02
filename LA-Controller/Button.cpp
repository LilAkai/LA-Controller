#include "pch.h"
#include "HID/hidapi.h"
#include "../Packages/LA-Controller/Button.h"
#include "../Packages/LA-Controller/Controller.h"

la::Button::Button(Controller &controller): parent(controller) {
    // Initialize all buttons as not pressed
    for (int i = 0; i<maxButtons; i++) {
        buttonStates[i] = false;
    }
}

bool la::Button::isPressed(const la::ControllerType::Dualsense button) {
    if (!&parent||!parent.isConnected()) {
        return false;
    }

    updateButtonStates();

    unsigned int buttonIndex = static_cast<unsigned int>(button);
    if (buttonIndex>=maxButtons) {
        return false;
    }

    return buttonStates[buttonIndex];
}

bool la::Button::isPressed(la::ControllerType::Xbox button) {
    if (!&parent||!parent.isConnected()) {
        return false;
    }

    updateButtonStates();

    unsigned int buttonIndex = static_cast<unsigned int>(button);
    if (buttonIndex>=maxButtons) {
        return false;
    }

    return buttonStates[buttonIndex];
}

void la::Button::updateButtonStates() {
    if (!&parent||!parent.isConnected()) {
        // If no controller, all buttons are released
        for (int i = 0; i<maxButtons; i++) {
            buttonStates[i] = false;
        }
        return;
    }

    // Read the HID button data
    readHIDButtonData();
}

void la::Button::readHIDButtonData() {
    if (!&parent) return;

    hid_device *device = parent.getIdentification().getDevice();
    if (!device) return;

    if (parent.isDualSense()) {
        // Read input report
        unsigned char inputData[64];
        int bytesRead = hid_read_timeout(device, inputData, sizeof(inputData), 10); // 10ms timeout

        if (bytesRead>0&&inputData[0]==0x01) {
            unsigned char face = inputData[8];
            // Croix, Rond, Carré, Triangle
            buttonStates[la::ControllerType::Square] = (face&0x10)!=0;
            buttonStates[la::ControllerType::Cross] = (face&0x20)!=0;
            buttonStates[la::ControllerType::Circle] = (face&0x40)!=0;
            buttonStates[la::ControllerType::Triangle] = (face&0x80)!=0;
            // D-pad (hat), à mapper selon besoin : 
            // Up:   ((face & 0x0F) == 0), Right: ==2, Down: ==4, Left: ==6, etc.

            unsigned char misc = inputData[9];
            buttonStates[la::ControllerType::L1] = (misc&0x01)!=0;
            buttonStates[la::ControllerType::R1] = (misc&0x02)!=0;
            /*buttonStates[la::ControllerType::L2] = (misc&0x04)!=0;
            buttonStates[la::ControllerType::R2] = (misc&0x08)!=0;*/
            buttonStates[la::ControllerType::Share] = (misc&0x10)!=0;
            buttonStates[la::ControllerType::Options] = (misc&0x20)!=0;
            buttonStates[la::ControllerType::LStick_DS] = (misc&0x40)!=0;
            buttonStates[la::ControllerType::RStick_DS] = (misc&0x80)!=0;

            unsigned char extra = inputData[10];
            buttonStates[la::ControllerType::PS] = (extra&0x01)!=0;
            buttonStates[la::ControllerType::TouchPad] = (extra&0x02)!=0;
            buttonStates[la::ControllerType::Mute] = (extra&0x04)!=0;

            // unsigned char edgePaddles = inputData[11];
            // buttonStates[la::ControllerType::PaddleL] = (edgePaddles & 0x01) != 0;
            // buttonStates[la::ControllerType::PaddleR] = (edgePaddles & 0x02) != 0;
        }
    } else if (parent.isXboxController()) {
        unsigned char inputData[64];
        int bytesRead = hid_read_timeout(device, inputData, sizeof(inputData), 10);

        if (bytesRead>0&&inputData[0]==0x01) {
            unsigned char buttons1 = inputData[2];

            buttonStates[la::ControllerType::A] = (buttons1&0x01)!=0;
            buttonStates[la::ControllerType::B] = (buttons1&0x02)!=0;
            buttonStates[la::ControllerType::X] = (buttons1&0x04)!=0;
            buttonStates[la::ControllerType::Y] = (buttons1&0x08)!=0;

            buttonStates[la::ControllerType::LB] = (buttons1&0x10)!=0;
            buttonStates[la::ControllerType::RB] = (buttons1&0x20)!=0;

            unsigned char buttons2 = inputData[3];
            buttonStates[la::ControllerType::Select] = (buttons2&0x01)!=0;
            buttonStates[la::ControllerType::Start] = (buttons2&0x02)!=0;
            buttonStates[la::ControllerType::LStick_Xbox] = (buttons2&0x04)!=0;
            buttonStates[la::ControllerType::RStick_Xbox] = (buttons2&0x08)!=0;
            buttonStates[la::ControllerType::Home] = (buttons2&0x10)!=0;

            // Palettes Elite = inputData[5] (en général)
            // unsigned char paddles = inputData[5];
            // buttonStates[la::ControllerType::Paddle1] = (paddles & 0x01) != 0;
            // buttonStates[la::ControllerType::Paddle2] = (paddles & 0x02) != 0;
            // buttonStates[la::ControllerType::Paddle3] = (paddles & 0x04) != 0;
            // buttonStates[la::ControllerType::Paddle4] = (paddles & 0x08) != 0;
        }
    }
}