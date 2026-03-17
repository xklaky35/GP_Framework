#ifndef GP_FRAMEWORK_CIRCLE_H
#define GP_FRAMEWORK_CIRCLE_H

#include "form.h"

namespace Engine {
    class Circle : public Form {
    public:
        Circle(const Transform &t);
        ~Circle() override;
        float GetArea() override;
        Vector2d GetFurthestPointTowards(Vector2d) const override;
    };
}
#endif //GP_FRAMEWORK_CIRCLE_H