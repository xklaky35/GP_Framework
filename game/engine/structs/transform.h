#ifndef GP_FRAMEWORK_TRANSFORM_H
#define GP_FRAMEWORK_TRANSFORM_H
#include "vector2d.h"

namespace Engine {
    struct Transform {
        Vector2d* position;
        float scale;
        float rotation;

        Transform() : scale(1), rotation(0) {
            position = new Vector2d();
        }
        ~Transform() {
            delete position;
            position = nullptr;
        }
    };
}

#endif //GP_FRAMEWORK_TRANSFORM_H