#include "pch.h"
#include "LA-Controller/Vector.h"

#include <cmath>

// Constructeur par défaut (déjà présent via = default dans .h)

// Constructeur avec paramètres - DÉFINITION AJOUTÉE
la::Vector2f::Vector2f(float x, float y) {
    this->x = x;
    this->y = y;
}

la::Vector2f la::Vector2f::normalized() const {
    float lenSq = x*x+y*y;
    if (lenSq>0.0f) {
        float invLen = 1.0f/std::sqrt(lenSq);
        return Vector2f(x*invLen, y*invLen);
    }
    return Vector2f(0.0f, 0.0f); // vecteur nul
}

la::Vector2f la::Vector2f::operator+(const Vector2f &other) const {
    return la::Vector2f(this->x+other.x,this->y+other.y);
}

la::Vector2f la::Vector2f::operator-(const Vector2f &other) const {
    return la::Vector2f(other.x-this->x,other.y-this->y);
}

la::Vector2f la::Vector2f::operator*(float scalar) const {
    return la::Vector2f(this->x*scalar,this->y*scalar);
}

la::Vector2f la::Vector2f::operator/(float scalar) const {
    return la::Vector2f(this->x/scalar,this->y/scalar);
}

float la::Vector2f::length() const {
    return std::sqrt(x*x+y*y);
}