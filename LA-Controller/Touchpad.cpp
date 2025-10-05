#include "pch.h"
#include "../Packages/LA-Controller/Export.h"
#include "../Packages/LA-Controller/Touchpad.h"

namespace la {

    Touchpad::Touchpad(): position(0.0f, 0.0f), touched(false) {}

    Touchpad::~Touchpad() {}

    la::Vector2f Touchpad::getPosition() const {
        return position;
    }

    bool Touchpad::isTouched() const {
        return touched;
    }

    void Touchpad::updatePosition(float x, float y) {
        position.x = x;
        position.y = y;
    }

    void Touchpad::updateTouchState(bool isTouched) {
        touched = isTouched;
    }

    void Touchpad::updateFromHID(const unsigned char *inputData, size_t length) {
        // Vérifier que le buffer a assez de données
        if (length<36) { // minimum pour extraire infos touchpad usuelles
            touched = false;
            position = la::Vector2f(0.0f, 0.0f);
            return;
        }

        // Exemple typique pour DualSense USB (HID report 0x01):
        // inputData[32] = flag touches (bit 0 : touchpad touched)
        // inputData[33..34] = X du contact 1 (little endian uint16_t)
        // inputData[35..36] = Y du contact 1 (little endian uint16_t)

        touched = (inputData[32]&0x01)!=0;

        uint16_t x = inputData[33]|(inputData[34]<<8);
        uint16_t y = inputData[35]|(inputData[36]<<8);

        // Normaliser entre 0 et 1 (selon specs : X max ~1920, Y max ~940)
        position.x = static_cast<float>(x)/1920.0f;
        position.y = static_cast<float>(y)/940.0f;
    }

}
