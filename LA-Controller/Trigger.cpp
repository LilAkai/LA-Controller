#include "pch.h"
#include "HID/hidapi.h"

#include "../Packages/LA-Controller/Trigger.h"
#include "../Packages/LA-Controller/Controller.h"
#include "../Packages/LA-Controller/ControllerType.h" // si besoin de l'enum ControllerType

namespace la {

    Trigger::Trigger(Controller &parent): parent(parent), value(0.0f) {}

    float Trigger::getValue() {
        return this->value;
    }

    void Trigger::updateState(unsigned int index) {
        readHIDAxisData(index);
    }

    void Trigger::readHIDAxisData(unsigned int index) {
        if (!&parent||!parent.isConnected()) {
            value = 0.0f;
            return;
        }

        hid_device *device = parent.getIdentification().getDevice();
        if (!device) {
            value = 0.0f;
            return;
        }

        unsigned char inputData[64];
        int bytesRead = hid_read_timeout(device, inputData, sizeof(inputData), 10);

        if (bytesRead>0) {

            if (parent.isDualSense()) {
                // DualSense : L2 = inputData[5], R2 = inputData[6]
                if (index==0)          // L2
                    value = static_cast<float>(inputData[5])/255.0f;
                else if (index==1)     // R2
                    value = static_cast<float>(inputData[6])/255.0f;
                else
                    value = 0.0f;
            } else if (parent.isXboxController()) {
                // Xbox : LT/RT sont souvent inputData[4]/[5] (à vérifier selon HID réel)
                if (index==0)          // LT
                    value = static_cast<float>(inputData[4])/255.0f;
                else if (index==1)     // RT
                    value = static_cast<float>(inputData[5])/255.0f;
                else
                    value = 0.0f;
            } else {
                value = 0.0f;
            }
        }
    }
}