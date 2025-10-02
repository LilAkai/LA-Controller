#include "pch.h"
#include "../Packages/LA-Controller/DPAD.h"
#include "../Packages/LA-Controller/Controller.h"

namespace la {

    DPAD::DPAD(Controller &controller): parent(controller), dpadRawDirection(8) {}

    void DPAD::update(unsigned char rawValue) {
        dpadRawDirection = rawValue&0x0F;
        if (dpadRawDirection>7) {
            dpadRawDirection = 8; // Neutre
        }
    }

    DpadDirection DPAD::getDirection() const {
        return static_cast<DpadDirection>(dpadRawDirection);
    }

    bool DPAD::isNeutral() const {
        return dpadRawDirection==8;
    }

    bool DPAD::isUp() const {
        return dpadRawDirection==0||dpadRawDirection==1||dpadRawDirection==7;
    }

    bool DPAD::isRight() const {
        return dpadRawDirection==1||dpadRawDirection==2||dpadRawDirection==3;
    }

    bool DPAD::isDown() const {
        return dpadRawDirection==3||dpadRawDirection==4||dpadRawDirection==5;
    }

    bool DPAD::isLeft() const {
        return dpadRawDirection==5||dpadRawDirection==6||dpadRawDirection==7;
    }
}
