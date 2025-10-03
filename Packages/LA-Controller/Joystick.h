#pragma once
#include "Export.h"
#include "Vector.h"

namespace la {
    enum stickType {
        LeftStick,
        RightStick
	};

    class Controller;

    class LA_CONTROLLER_API Joystick {
    private: /*--VARIABLES--*/
        static constexpr unsigned int axisCount {8};
        Controller &parent; // Référence vers le contrôleur parent

        la::Vector2f axes[axisCount]; // Pour sticks et triggers

    private: /*--FUNCTIONS--*/
        void readHIDAxisData();

    public: /*--FUNCTIONS--*/
        Joystick(Controller &controller);
        Joystick() = default;
        ~Joystick() = default;

        /*
		@brief Acces to joystick's axes
		@param axisIndex Index of the axis (0 or 1)
		@return Vector2f representing the axis state (-1.0 to 1.0 for both x and y)
        */
        [[nodiscard]] la::Vector2f getAxis(unsigned int axisIndex);

        // Mise à jour axes + Dpad via HID
        void updateAxisStates();

    };
}
