#include "Vector.h"

namespace la {

    class Touchpad {
    private:
        la::Vector2f position;
        bool touched;           // true si touch�
        bool touchDataUpdated;  // flag pour savoir si on a re�u des donn�es r�centes

    public:
        Touchpad();
        ~Touchpad();

        la::Vector2f getPosition() const;
        bool isTouched() const;

        // Met � jour la position et l'�tat du touchpad, par ex depuis rapport HID
        void updatePosition(float x, float y);
        void updateTouchState(bool isTouched);

        inline void update() {
            this->updatePosition(this->position.x, this->position.y);
            this->updateTouchState(this->touched);
        }

        // Mets � jour les donn�es compl�tes � partir d'un buffer HID lors d'une lecture
        void updateFromHID(const unsigned char *inputData, size_t length);
    };
}
