#ifndef GP_FRAMEWORK_VECTOR2D_H
#define GP_FRAMEWORK_VECTOR2D_H

namespace Engine {
    struct Vector2d {
        float x;
        float y;

        Vector2d();
        Vector2d(float x, float y);

        float Distance(const Vector2d&) const;
        Vector2d Zero();
        Vector2d operator-(const Vector2d&) const;
        Vector2d operator+(const Vector2d&) const;
        Vector2d operator+(float vector) const;
        Vector2d& operator+=(float);
        Vector2d& operator+=(const Vector2d&);
        Vector2d operator*(float vector) const;
        bool operator==(const Vector2d&) const;
        bool operator!=(const Vector2d&) const;
    };
}

#endif //GP_FRAMEWORK_VECTOR2D_H