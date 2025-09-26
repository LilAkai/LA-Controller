#include "pch.h"
#include "Trigger.h"

la::Trigger::Trigger(): value(0.0f) {}

float la::Trigger::getValue() {
    return this->value;
}