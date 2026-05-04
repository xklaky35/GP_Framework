#include "vector3d.h"

#include <cmath>

namespace Engine {
    Vector3d::Vector3d() : x(0), y(0), z(0) {
    }

    Vector3d::Vector3d(const float x, const float y, float z) : x(x), y(y), z(z) {
    }

    Vector3d Vector3d::Zero() {
        return Vector3d(0,0,0);
    }

    Vector3d Vector3d::operator-(const Vector3d &vector) const {
        return Vector3d{vector.x - x, vector.y - y, vector.z - z};
    }

    Vector3d Vector3d::operator+(const Vector3d &vector) const {
        return Vector3d{vector.x + x, vector.y + y, vector.z + z};
    }

    Vector3d Vector3d::operator+(const float vector) const {
        return Vector3d{x + vector, y + vector, z + vector};
    }

    Vector3d & Vector3d::operator+=(float f) {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    Vector3d & Vector3d::operator-=(float f) {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    Vector3d& Vector3d::operator+=(const Vector3d& vector) {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        return *this;
    }

    Vector3d & Vector3d::operator-=(const Vector3d& vector) {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        return *this;
    }

    Vector3d Vector3d::operator*(float vector) const {
        return Vector3d{x * vector, y * vector, z * vector};
    }

    Vector3d Vector3d::operator*(Vector3d vector) const {
        return Vector3d{x * vector.x, y * vector.y, z * vector.z};
    }

    bool Vector3d::operator==(const Vector3d &vector) const {
        if (x == vector.x && y == vector.y && z == vector.z) return true;
        return false;
    }

    bool Vector3d::operator!=(const Vector3d &vector) const {
        if (*this == vector) return true;
        return false;
    }

    Vector3d& Vector3d::operator/=(const float f) {
        x /= f;
        y /= f;
        z /= f;
        return *this;
    }

    float Vector3d::Length() const {
        return static_cast<float>(std::sqrt(pow(x,2) + pow(y,2) + pow(z,2)));
    }

    Vector3d Vector3d::Normalized(const Vector3d vec) {
        return Vector3d{vec.x/vec.Length(), vec.y/vec.Length(), vec.z/vec.Length()};
    }

    Vector3d Vector3d::Cross(const Vector3d a, const Vector3d b) {
        return Vector3d{
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }
}
