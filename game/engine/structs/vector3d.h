//
// Created by leon on 03.05.26.
//

#ifndef GP_FRAMEWORK_VECTOR3D_H
#define GP_FRAMEWORK_VECTOR3D_H


namespace Engine {
    struct Vector3d {
        float x;
        float y;
        float z;

        Vector3d();
        Vector3d(float x, float y, float z);

        static Vector3d Zero();
        static Vector3d Normalized(Vector3d);
        static Vector3d Cross(Vector3d, Vector3d);
        Vector3d operator-(const Vector3d&) const;
        Vector3d operator+(const Vector3d&) const;
        Vector3d operator+(float vector) const;
        Vector3d& operator+=(float);
        Vector3d& operator-=(float);
        Vector3d& operator+=(const Vector3d&);
        Vector3d& operator-=(const Vector3d&);
        Vector3d operator*(float vector) const;
        Vector3d operator*(Vector3d vector) const;
        bool operator==(const Vector3d&) const;
        bool operator!=(const Vector3d&) const;
        Vector3d& operator/=(float);

        [[nodiscard]] float Length() const;
    };
}


#endif //GP_FRAMEWORK_VECTOR3D_H