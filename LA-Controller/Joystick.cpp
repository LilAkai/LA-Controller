#include "pch.h"
#include "LA-Controller/Joystick.h"

la::Joystick::Joystick(): axis() {}

la::Vector2f la::Joystick::getAxis() {
    return this->axis;
}