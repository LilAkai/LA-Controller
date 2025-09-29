#include "pch.h"
#include "Joystick.h"

la::Joystick::Joystick(): axis() {}

la::Vector2f la::Joystick::getAxis() {
    return this->axis;
}