#include "Vector.h"

namespace la {

    class Touchpad {
    private:
        la::Vector2f position;
        bool touched;           // true si touché
        bool touchDataUpdated;  // flag pour savoir si on a reçu des données récentes

    public:
        Touchpad();
        ~Touchpad();

        la::Vector2f getPosition() const;
        bool isTouched() const;

        // Met à jour la position et l'état du touchpad, par ex depuis rapport HID
        void updatePosition(float x, float y);
        void updateTouchState(bool isTouched);

        inline void update() {
            this->updatePosition(this->position.x, this->position.y);
            this->updateTouchState(this->touched);
        }

        // Mets à jour les données complètes à partir d'un buffer HID lors d'une lecture
        void updateFromHID(const unsigned char *inputData, size_t length);
    };
}
