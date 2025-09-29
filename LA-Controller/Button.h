#pragma once
#include "Export.h"
#include "ControllerType.h"

namespace la {
    class Controller;
    class LA_CONTROLLER_API Button {
    private:
        static constexpr unsigned int maxButtons{ 32 };
        Controller* parent; // Référence vers le contrôleur parent

        // Buffer pour stocker l'état des boutons lu depuis HID
        bool buttonStates[maxButtons];

    public:
        // Constructeur avec référence au controller parent
        Button(Controller* controller = nullptr);
        ~Button() = default;

        // Méthodes pour lire l'état des boutons
        [[nodiscard]] bool isPressed(la::ControllerType::Dualsense button);
        [[nodiscard]] bool isPressed(la::ControllerType::Xbox button);

        // Méthode pour mettre à jour l'état des boutons depuis HID
        void updateButtonStates();

        // Méthode interne pour lire les données HID
    private:
        void readHIDButtonData();
    };
}
