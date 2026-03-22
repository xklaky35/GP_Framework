#ifndef GP_FRAMEWORK_TRANSFORM_H
#define GP_FRAMEWORK_TRANSFORM_H
#include "vector2d.h"

namespace Engine {
    struct Transform {

        Transform() : scale(1), rotation(0) {}
        ~Transform() = default;

        void SetScale(float scale);
        void SetWidth(float w);
        void SetHeight(float h);
        void SetSize(float w, float h);
        void SetRotation(float degrees);

        float GetScale() const;
        float GetRotationDeg() const;
        float GetRotationRad() const;
        float GetWidth() const;
        float GetHeight() const;

        Transform& operator=(const Transform&);

        Vector2d position;
        Vector2d scaledSize;
        Vector2d baseSize;
    private:
        float scale;
        float rotation;

    };
}

#endif //GP_FRAMEWORK_TRANSFORM_H