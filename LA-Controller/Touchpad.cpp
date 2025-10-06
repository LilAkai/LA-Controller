#include "pch.h"
#include <cmath>
#include <algorithm>

#include "../Packages/LA-Controller/Touchpad.h"
#include "../Packages/LA-Controller/Controller.h"

namespace la {

    Touchpad::Touchpad(Controller &parent)
        : parent(parent)
        , position(0.f, 0.f)
        , touched(false)
        , touchDataUpdated(false) {}

    Touchpad::~Touchpad() {}

    la::Vector2f Touchpad::getPosition() const {
        return position;
    }

    bool Touchpad::isTouched() const {
        return touched;
    }

    void Touchpad::updatePosition(la::Vector2f vec) {
        position = vec;
        touchDataUpdated = true;
    }

    void Touchpad::updateTouchState(bool isTouched) {
        touched = isTouched;
        touchDataUpdated = true;
    }

    void Touchpad::update() {
        this->readHIDTouchpadData();
        this->updateTouchState(this->touched);
        this->updatePosition(this->position);
    }

    void Touchpad::readHIDTouchpadData() {
        if (!&parent||!parent.isConnected()) {
            touched = false;
            position = la::Vector2f(0.f, 0.f);
            touchDataUpdated = false;
            return;
        }

        hid_device *device = parent.getIdentification().getDevice();
        if (!device) return;

        unsigned char inputData[64];
        int bytesRead = hid_read_timeout(device, inputData, sizeof(inputData), 10);

        if (bytesRead>0&&inputData[0]==0x01) {
            updateFromHID(inputData, bytesRead);
        }
    }

    void Touchpad::updateFromHID(const unsigned char *inputData, size_t length) {
        if (length<37) {
            touched = false;
            position = la::Vector2f(0.f, 0.f);
            touchDataUpdated = false;
            return;
        }

        touched = (inputData[32]&0x01)!=0;

        uint16_t x = inputData[33]|(static_cast<uint16_t>(inputData[34])<<8);
        uint16_t y = inputData[35]|(static_cast<uint16_t>(inputData[36])<<8);

        // Valeurs maximales à ajuster en fonction du touchpad
        constexpr float x_max = 65535.f; // typique max uint16
        constexpr float y_max = 65535.f;

        // Normalisation dans [0,1]
        position.x = static_cast<float>(x)/x_max;
        position.y = static_cast<float>(y)/y_max;

        touchDataUpdated = true;
    }
}