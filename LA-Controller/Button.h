#pragma once
#include "Export.h"
#include "ControllerType.h"

namespace la {
    class Controller;
    class LA_CONTROLLER_API Button {
    private:
        static constexpr unsigned int maxButtons{ 32 };
        Controller* parent; // R�f�rence vers le contr�leur parent

        // Buffer pour stocker l'�tat des boutons lu depuis HID
        bool buttonStates[maxButtons];

    public:
        // Constructeur avec r�f�rence au controller parent
        Button(Controller* controller = nullptr);
        ~Button() = default;

        // M�thodes pour lire l'�tat des boutons
        [[nodiscard]] bool isPressed(la::ControllerType::Dualsense button);
        [[nodiscard]] bool isPressed(la::ControllerType::Xbox button);

        // M�thode pour mettre � jour l'�tat des boutons depuis HID
        void updateButtonStates();

        // M�thode interne pour lire les donn�es HID
    private:
        void readHIDButtonData();
    };
}
