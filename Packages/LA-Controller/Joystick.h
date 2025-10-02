#pragma once
#include "Export.h"
#include "Vector.h"

namespace la {
    class Controller;

    class LA_CONTROLLER_API Joystick {
    private: /*--VARIABLES--*/
        static constexpr unsigned int axisCount {8};
        Controller &parent; // R�f�rence vers le contr�leur parent

        la::Vector2f axes[axisCount]; // Pour sticks et triggers
        int dpadDirection = 8; // Neutre par d�faut (aucun appui)

    private: /*--FUNCTIONS--*/
        void readHIDAxisData();

    public: /*--FUNCTIONS--*/
        Joystick(Controller &controller);
        Joystick() = default;
        ~Joystick() = default;

        // Acc�s axes sticks analogiques
        [[nodiscard]] la::Vector2f getAxis(unsigned int axisIndex);

        // Acc�s D-Pad num�rique (0-7 directions, 8 = neutre)
        [[nodiscard]] int getDpadDirection();

        // Mise � jour axes + Dpad via HID
        void updateAxisStates();
    };
}
