#include "pch.h"
#include "LA-Controller/Trigger.h"

la::Trigger::Trigger(): value(0.0f) {}

float la::Trigger::getValue() {
    return this->value;
}