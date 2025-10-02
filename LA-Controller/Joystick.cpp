#include "pch.h"
#include "HID/hidapi.h"

#include "../Packages/LA-Controller/Joystick.h"
#include "../Packages/LA-Controller/Controller.h"

namespace la {
        
    Joystick::Joystick(Controller &controller): parent(controller), axes {la::Vector2f(0.0f, 0.0f), la::Vector2f(0.0f, 0.0f)}, dpadDirection(8) {
    }

    la::Vector2f Joystick::getAxis(unsigned int axisIndex) {
        if (axisIndex>=axisCount)
            return la::Vector2f(0.0f, 0.0f);
        return axes[axisIndex];
    }

    int Joystick::getDpadDirection() {
        return dpadDirection;
    }

    void Joystick::updateAxisStates() {
        readHIDAxisData();
    }

    void la::Joystick::readHIDAxisData() {
        if (!&parent||!parent.isConnected()) {
            for (unsigned int i = 0; i<axisCount; ++i)
                axes[i] = la::Vector2f(0.0f, 0.0f);
            dpadDirection = 8;
            return;
        }

        hid_device *device = parent.getIdentification().getDevice();
        if (!device) return;

        unsigned char inputData[64];
        int bytesRead = hid_read_timeout(device, inputData, sizeof(inputData), 10);

        if (bytesRead>0&&inputData[0]==0x01) {
            auto normalize = [](unsigned char val) -> float {
                float n = (static_cast<float>(val)-128.0f)/127.0f;
                if (n>1.0f) n = 1.0f;
                if (n<-1.0f) n = -1.0f;
                return n;
            };


            // Normalize X/Y des sticks sur [-1, 1] avec 0 au centre (val=128)
            axes[0] = la::Vector2f(normalize(inputData[1]), -normalize(inputData[2])); // stick gauche (inversion Y si besoin)
            axes[1] = la::Vector2f(normalize(inputData[3]), -normalize(inputData[4])); // stick droit

            // D-Pad
            dpadDirection = inputData[8]&0x0F;
        }
    }
}
