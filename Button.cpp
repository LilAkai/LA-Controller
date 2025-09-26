#include "pch.h"
#include "Button.h"
#include "HID/hidapi.h"

// Implémentation complète de Button - à ajouter dans Controller.cpp

la::Button::Button(Controller *controller): parent(controller) {
    // Initialiser tous les boutons comme non pressés
    for (int i = 0; i<maxButtons; i++) {
        buttonStates[i] = false;
    }
}

bool la::Button::isPressed(la::ControllerType::Dualsense button) {
    if (!parent||!parent->isConnected()) {
        return false;
    }

    // Mettre à jour l'état des boutons
    updateButtonStates();

    // Vérifier si l'index du bouton est valide
    unsigned int buttonIndex = static_cast<unsigned int>(button);
    if (buttonIndex>=maxButtons) {
        return false;
    }

    return buttonStates[buttonIndex];
}

bool la::Button::isPressed(la::ControllerType::Xbox button) {
    if (!parent||!parent->isConnected()) {
        return false;
    }

    // Mettre à jour l'état des boutons
    updateButtonStates();

    // Vérifier si l'index du bouton est valide
    unsigned int buttonIndex = static_cast<unsigned int>(button);
    if (buttonIndex>=maxButtons) {
        return false;
    }

    return buttonStates[buttonIndex];
}

void la::Button::updateButtonStates() {
    if (!parent||!parent->isConnected()) {
        // Si pas de contrôleur, tous les boutons sont relâchés
        for (int i = 0; i<maxButtons; i++) {
            buttonStates[i] = false;
        }
        return;
    }

    // Lire les données depuis le contrôleur HID
    readHIDButtonData();
}

void la::Button::readHIDButtonData() {
    if (!parent) return;

    hid_device *device = parent->getIdentification().getDevice();
    if (!device) return;

    if (parent->isDualSense()) {
        // Lecture des données DualSense
        unsigned char inputData[64];
        int bytesRead = hid_read_timeout(device, inputData, sizeof(inputData), 10); // 10ms timeout

        if (bytesRead>0) {
            // DualSense input report format (USB mode)
            if (inputData[0]==0x01) { // Report ID pour input standard
                // Les boutons sont dans les octets 5 et 6
                unsigned char buttons1 = inputData[5]; // Triangle, Circle, Cross, Square, etc.
                unsigned char buttons2 = inputData[6]; // L1, R1, L2, R2, etc.
                unsigned char buttons3 = inputData[7]; // Options, Share, etc.

                // Décoder les boutons individuels
                buttonStates[la::ControllerType::Cross] = (buttons1&0x01)!=0;
                buttonStates[la::ControllerType::Circle] = (buttons1&0x02)!=0;
                buttonStates[la::ControllerType::Square] = (buttons1&0x04)!=0;
                buttonStates[la::ControllerType::Triangle] = (buttons1&0x08)!=0;

                buttonStates[la::ControllerType::L1] = (buttons2&0x01)!=0;
                buttonStates[la::ControllerType::R1] = (buttons2&0x02)!=0;

                buttonStates[la::ControllerType::Share] = (buttons3&0x01)!=0;
                buttonStates[la::ControllerType::Options] = (buttons3&0x02)!=0;
                buttonStates[la::ControllerType::LStick_DS] = (buttons3&0x04)!=0;
                buttonStates[la::ControllerType::RStick_DS] = (buttons3&0x08)!=0;
            }
        }

    } else if (parent->isXboxController()) {
        // Lecture des données Xbox Controller
        unsigned char inputData[64];
        int bytesRead = hid_read_timeout(device, inputData, sizeof(inputData), 10);

        if (bytesRead>0) {
            // Xbox input report format
            if (inputData[0]==0x01) { // Report ID
                unsigned char buttons1 = inputData[2];
                unsigned char buttons2 = inputData[3];

                // Décoder les boutons Xbox
                buttonStates[la::ControllerType::A] = (buttons1&0x01)!=0;
                buttonStates[la::ControllerType::B] = (buttons1&0x02)!=0;
                buttonStates[la::ControllerType::X] = (buttons1&0x04)!=0;
                buttonStates[la::ControllerType::Y] = (buttons1&0x08)!=0;

                buttonStates[la::ControllerType::LB] = (buttons1&0x10)!=0;
                buttonStates[la::ControllerType::RB] = (buttons1&0x20)!=0;

                buttonStates[la::ControllerType::Select] = (buttons2&0x01)!=0;
                buttonStates[la::ControllerType::Start] = (buttons2&0x02)!=0;
                buttonStates[la::ControllerType::LStick_Xbox] = (buttons2&0x04)!=0;
                buttonStates[la::ControllerType::RStick_Xbox] = (buttons2&0x08)!=0;
            }
        }
    }
}