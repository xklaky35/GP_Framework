#ifndef GP_FRAMEWORK_FORMNODE_H
#define GP_FRAMEWORK_FORMNODE_H

#include "../../structs/transform.h"
#include "../../structs/vector2d.h"

namespace Engine {
    class Form {
    public:
        Form(const Transform& t) {
            m_transform = t;
        }
        virtual ~Form() = default;

        virtual float GetArea() = 0;
        virtual bool Overlap(const Form &) const;
        virtual Vector2d GetFurthestPointTowards(Vector2d) const = 0;

    public:
        Transform m_transform;
    };



}
#endif //GP_FRAMEWORK_FORMNODE_H
