#include "rectangle.h"

namespace Engine {
    Rectangle::Rectangle(Vector2d pos, const float size) : Form(pos, size, size) {}
    Rectangle::~Rectangle() = default;

    float Rectangle::GetArea() {
        return 0.f;
    }

    bool Rectangle::Overlap(const Form &) const {
        return false;
    }

    Vector2d Rectangle::GetFurthestPointTowards(const Vector2d) const {
        return Vector2d(0,0);
    }

}


