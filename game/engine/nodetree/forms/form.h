#ifndef GP_FRAMEWORK_FORMNODE_H
#define GP_FRAMEWORK_FORMNODE_H

#include "../../structs/vector2d.h"

namespace Engine {
    class Form {
    public:
        Form(Vector2d* position) : m_position(position) {}
        virtual ~Form() = default;

        virtual float GetArea() = 0;
        virtual bool Overlap(Form &) = 0;
        virtual Vector2d GetFurthestPointTowards(const Vector2d*) const = 0;

    public:
        Vector2d* m_position;
    };
}
#endif //GP_FRAMEWORK_FORMNODE_H
