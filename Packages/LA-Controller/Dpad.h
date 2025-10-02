#pragma once
#include "Export.h"

namespace la {

    enum class LA_CONTROLLER_API DpadDirection {
        Neutral = 8,
        Up = 0,
        UpRight = 1,
        Right = 2,
        DownRight = 3,
        Down = 4,
        DownLeft = 5,
        Left = 6,
        UpLeft = 7
    };


    class Controller;

    class LA_CONTROLLER_API DPAD {
    private:
        unsigned char dpadRawDirection = 8; // 8 == neutre
        Controller &parent;
    public:
        DPAD(Controller &parent);
        DPAD() = default;
        ~DPAD() = default;

        // Met à jour la direction brute lue depuis HID
        void update(unsigned char rawValue);

        // Retourne la direction sous forme énumérée
        [[nodiscard]] DpadDirection getDirection() const;

        // Méthodes utilitaires pour les directions spécifiques
        bool isNeutral() const;
        bool isUp() const;
        bool isRight() const;
        bool isDown() const;
        bool isLeft() const;
    };
}
