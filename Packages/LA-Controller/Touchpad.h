#pragma once

#include "Export.h"
#include "Vector.h"
#include <cstddef>

namespace la {

    class Controller; // Déclaration anticipée

    class LA_CONTROLLER_API Touchpad {
    private:
        Controller &parent;

        la::Vector2f position;
        bool touched;
        bool touchDataUpdated;

    public:
        Touchpad(Controller &parent);
        ~Touchpad();

        la::Vector2f getPosition() const;
        bool isTouched() const;

        void update();

    private:
        // Mets à jour les données complètes à partir d'un buffer HID lors d'une lecture
        void updateFromHID(const unsigned char *inputData, size_t length);

        void updatePosition(la::Vector2f vec);
        void updateTouchState(bool isTouched);

        // Lit les données HID du touchpad depuis le contrôleur parent
        void readHIDTouchpadData();
    };
}