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
        Rectangle(Vector2d pos, float height, float width);
        ~Rectangle() override;
        float GetArea() override;
        Vector2d GetFurthestPointTowards(Vector2d) const override;
    };

}


#endif //GP_FRAMEWORK_RECTANGLE_H
