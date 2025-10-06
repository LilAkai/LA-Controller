#pragma once

#include "Export.h"
#include "Vector.h"
#include <cstddef>

namespace la {

    class Controller; // D�claration anticip�e

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
        // Mets � jour les donn�es compl�tes � partir d'un buffer HID lors d'une lecture
        void updateFromHID(const unsigned char *inputData, size_t length);

        void updatePosition(la::Vector2f vec);
        void updateTouchState(bool isTouched);

        // Lit les donn�es HID du touchpad depuis le contr�leur parent
        void readHIDTouchpadData();
    };
}