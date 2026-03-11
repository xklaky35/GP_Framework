//
// Created by leon on 11.03.26.
//

#ifndef GP_FRAMEWORK_RECTANGLE_H
#define GP_FRAMEWORK_RECTANGLE_H

#include "form.h"
#include "../../structs/vector2d.h"

namespace Engine {
    class Rectangle : public Form {
    public:
        Rectangle(Vector2d* pos, Vector2d size);
        ~Rectangle() override;
        float GetArea() override;
        bool Overlap(Form &) override;
        Vector2d GetFurthestPointTowards(const Vector2d*) const override;

    public:
        Vector2d m_size;
    };

}


#endif //GP_FRAMEWORK_RECTANGLE_H
