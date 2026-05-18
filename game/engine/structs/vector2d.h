#ifndef GP_FRAMEWORK_VECTOR2D_H
#define GP_FRAMEWORK_VECTOR2D_H
#include "box2d/math_functions.h"

namespace Engine {
    struct Vector2d {
        float x;
        float y;

        Vector2d();
        Vector2d(float x, float y);

        [[nodiscard]] float Distance(const Vector2d&) const;
        [[nodiscard]] bool Compare(Vector2d vector2d, float i) const;

        static Vector2d Zero();
        operator b2Vec2() const;
        Vector2d operator-(const Vector2d&) const;
        Vector2d operator+(const Vector2d&) const;
        Vector2d operator+(float vector) const;
        Vector2d operator/(float i) const;
        Vector2d& operator+=(float);
        Vector2d& operator-=(float);
        Vector2d& operator+=(const Vector2d&);
        Vector2d& operator-=(const Vector2d&);
        Vector2d operator*(float vector) const;
        Vector2d operator*(Vector2d vector) const;
        bool operator==(const Vector2d&) const;
        bool operator!=(const Vector2d&) const;
        Vector2d& operator/=(float);
    };
}

#endif //GP_FRAMEWORK_VECTOR2D_H