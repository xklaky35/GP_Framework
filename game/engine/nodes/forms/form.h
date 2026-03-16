#ifndef GP_FRAMEWORK_FORMNODE_H
#define GP_FRAMEWORK_FORMNODE_H

#include "../../structs/vector2d.h"

namespace Engine {
    class Form {
    public:
        Form(Vector2d position, float h, float w) : m_position(position), m_height(h), m_width(w), m_scale(1.0f) {}
        virtual ~Form() = default;

        virtual float GetArea() = 0;
        virtual bool Overlap(const Form &) const;
        virtual Vector2d GetFurthestPointTowards(Vector2d) const = 0;

    public:
        Vector2d m_position;
        float m_height;
        float m_width;
        float m_scale;
    };



}
#endif //GP_FRAMEWORK_FORMNODE_H
