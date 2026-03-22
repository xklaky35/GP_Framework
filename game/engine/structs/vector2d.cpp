#include "vector2d.h"
#include <cmath>

namespace Engine {
    Vector2d::Vector2d() : x(0), y(0) {
    }

    Vector2d::Vector2d(const float x, const float y) : x(x), y(y) {
    }

    float Vector2d::Distance(const Vector2d &vec) const {
        return static_cast<float>(sqrt(pow(vec.x - x, 2) + pow(vec.y - y, 2)));
    }

    Vector2d Vector2d::Zero() {
        return Vector2d(0,0);
    }

    Vector2d Vector2d::operator-(const Vector2d &vector) const {
        return Vector2d{vector.x - x, vector.y - y};
    }

    Vector2d Vector2d::operator+(const Vector2d &vector) const {
        return Vector2d{vector.x + x, vector.y + y};
    }

    Vector2d Vector2d::operator+(const float vector) const {
        return Vector2d{x + vector, y + vector};
    }

    Vector2d & Vector2d::operator+=(float f) {
        x += f;
        y += f;
        return *this;
    }

    Vector2d& Vector2d::operator+=(const Vector2d& vector) {
        x += vector.x;
        y += vector.y;
        return *this;
    }

    Vector2d Vector2d::operator*(float vector) const {
        return Vector2d{x * vector, y * vector};
    }

    Vector2d Vector2d::operator*(Vector2d vector) const {
        return Vector2d{x * vector.x, y * vector.y};
    }

    bool Vector2d::operator==(const Vector2d &vector) const {
        if (x == vector.x && y == vector.y) return true;
        return false;
    }

    bool Vector2d::operator!=(const Vector2d &vector) const {
        if (*this == vector) return true;
        return false;
    }

    Vector2d& Vector2d::operator/=(const float f) {
        x /= f;
        y /= f;
        return *this;
    }
}
