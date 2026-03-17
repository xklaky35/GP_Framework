#include "transform.h"

namespace Engine {
    void Transform::SetScale(const float s) {
        scale = s;
    }

    void Transform::SetRotation(const float degrees) {
        rotation = degrees;
    }


    void Transform::SetSize(const float w, const float h) {
        scaledSize.x = w;
        scaledSize.y = h;

        baseSize.x = w / scale;
        baseSize.y = h / scale;
    }
    void Transform::SetHeight(const float h) {
        scaledSize.y = h;
        baseSize.y = h / scale;
    }
    void Transform::SetWidth(const float w) {
        scaledSize.x = w;
        baseSize.x = w / scale;
    }

    float Transform::GetScale() const {
        return scale;
    }

    float Transform::GetRotation() const {
        return rotation;
    }

    float Transform::GetWidth() const {
        return baseSize.x * scale;
    }

    float Transform::GetHeight() const {
        return baseSize.y * scale;
    }

    Transform& Transform::operator=(const Transform& t) {
        position.x = t.position.x;
        position.y = t.position.y;
        SetScale(t.GetScale());
        SetRotation(t.GetRotation());
        SetSize(t.GetWidth(), t.GetHeight());
        return *this;
    }
}
