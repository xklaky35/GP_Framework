#ifndef GP_FRAMEWORK_TRANSFORM_H
#define GP_FRAMEWORK_TRANSFORM_H
#include "vector2d.h"

namespace Engine {
    struct Transform {
        Vector2d* position;
        float scale;
        float rotation;
        float height;
        float width;

        Transform() : scale(1), rotation(0), height(50), width(50) {
            position = new Vector2d();
        }
        ~Transform() {
            delete position;
            position = nullptr;
        }
    };
}

#endif //GP_FRAMEWORK_TRANSFORM_H