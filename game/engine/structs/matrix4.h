// COMP710 GP Framework 2025
#ifndef MATRIX4_H
#define MATRIX4_H
#include <SDL_opengl.h>

namespace Engine {
    struct Matrix4 {
        float m[4][4];
    };

    void SetZero(Matrix4 &mat);
    void SetIdentity(Matrix4 &mat);
    void CreateOrthoProjection(Matrix4 &mat, float width, float height);

}
#endif // MATRIX4_H