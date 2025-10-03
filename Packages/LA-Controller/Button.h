#pragma once
#include "Export.h"
#include "ControllerType.h"

namespace la {
    class Controller;


    class LA_CONTROLLER_API Button {
    private: /*--VARIABLES--*/
        static constexpr unsigned int maxButtons{ 32 };
		Controller &parent; // Reference to the parent controller

		// Buffer to store button states read from HID
        bool buttonStates[maxButtons];

    private: /*--FUNCTIONS--*/
        // Internal function to read HID data
        void readHIDButtonData();

    public: /*--FUNCTIONS--*/
		// Constructor with reference to parent controller
        Button(Controller &controller);
        Button() = default;
        ~Button() = default;

		// Functions to read button states
        [[nodiscard]] bool isPressed(la::ControllerType::Dualsense button);
        [[nodiscard]] bool isPressed(la::ControllerType::Xbox button);

		// Function to update button states from HID
        void updateButtonStates();
    };
}
