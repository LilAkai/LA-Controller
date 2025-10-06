#pragma once
#include "Export.h"

namespace la {
    class LA_CONTROLLER_API Vector2f {
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Construct the vector from cartesian coordinates
        ///
        /// \param x X coordinate
        /// \param y Y coordinate
        ///
        ////////////////////////////////////////////////////////////
        Vector2f(float x = 0.f, float y = 0.f);

        ////////////////////////////////////////////////////////////
        /// \brief Vector with same direction but length 1 <i><b>(floating-point)</b></i>.
        ///
        /// \pre `*this` is no zero vector.
        ///
        ////////////////////////////////////////////////////////////
        Vector2f normalized() const;

		Vector2f operator+(const Vector2f &other) const;
		Vector2f operator-(const Vector2f &other) const;
		Vector2f operator*(float scalar) const;
		Vector2f operator/(float scalar) const;

        float length() const;

        float x {}; //!< X coordinate of the vector
        float y {}; //!< Y coordinate of the vector
    };
}